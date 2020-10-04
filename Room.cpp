#include "Room.h"
#include "Monster.h"
#include "Common.h"

Room::Room(Spritesheet& spritesheet, int level, Room *next)
    : _spritesheet(spritesheet), _is_last(false), _paused(true), _level(level)
{
    _next = next;
    _blue_fire = make_ref<Timer>(4000, [&]() {
        if (!_paused) {
            auto pos = Vector2i{_player->x(), _player->y()};

            int m_count = 0;
            for (auto* e : _entities) {
                if (e->type() == Entity::Monster) {
                    m_count++;
                    auto* monster = e->as<Monster>();
                    auto rel = Vector2i{monster->x() - pos.x(), monster->y() - pos.y()};
                    
                    Vector2f dv = {0, 0};
                    if (std::abs(rel.x()) > std::abs(rel.y())) {
                        if (rel.x() > 0)
                            dv.setX(-FIRE_SPEED - ((_level-1)*5));
                        else
                            dv.setX(FIRE_SPEED + ((_level-1)*5));
                    } else {
                        if (rel.y() > 0)
                            dv.setY(-FIRE_SPEED - ((_level-1)*5));
                        else
                            dv.setY(FIRE_SPEED + ((_level-1)*5));
                    }
                    
                    createFire(_spritesheet.createSprite(monster->sprite().x() + dv.x()*2, 
                                                         monster->sprite().y() + dv.y()*2, 
                                                         64, 64, S_BLUE_FIRE), dv, Fire::Blue);
                }
            }
            
            if (m_count > 0)
                Audio::playSound(g_sounds.at(SO_FIRE).get(), 75);
        }
    }, true);
}

Room::~Room()
{}

void Room::setNextRoom(Room *next)
{
    _next = next;
}

void Room::addEntity(Entity* entity)
{
    _entities.push_back(entity);
}

void Room::forEachEntitiy(std::function<void(Entity*)> fn) {
    for (auto* e : _entities)
        fn(e);
}

void Room::setIsLast(bool state) {
    _is_last = state;
}

void Room::setDoor(Door *door) {
    _door = door;
}

void Room::createFire(Ref<Sprite> sprite, Vector2f dv, Fire::FireType type) {
    _fire.push_back(new Fire(sprite, dv, type));
}

void Room::renderFire(Matrix4f view) {
    for (auto* fire : _fire) 
        fire->sprite().render(view);
}

void Room::moveFire(float delta) {
    for (auto fire = _fire.begin(); fire < _fire.end(); fire++) {
        // Move fire
        auto dv = (*fire)->dv();
        dv = {dv.x() * delta, dv.y() * delta};
        (*fire)->sprite().translate(dv);
        
        // Check monster collisions
        for (auto e = _entities.begin(); e < _entities.end(); e++) {
            if ((*e)->type() == Entity::Monster && (*fire)->sprite().intersects((*e)->sprite())) {
                if ((*fire)->fireType() == Fire::Blue)
                    continue;
                
                auto* monster = (*e)->as<Monster>();
                if (monster->key()) {
                    _player->setItem(monster->key());
                    monster->key()->sprite().setPosition(0, 640);
                    monster->key()->sprite().setSize(64, 64);
                    Audio::playSound(g_sounds.at(SO_DROP).get(), 75);
                } else {
                    auto sound = g_sounds.at(SO_KILL);
                    Audio::playSound(sound.get(), 75);
                }
                
                e = _entities.erase(e);
                continue;
            }
            
            // Check player collisions
            if ((*fire)->sprite().intersects(_player->sprite())) {
                if ((*fire)->fireType() != Fire::Regular) {
                    _player->lowerHealth();
                    Audio::playSound(g_sounds.at(SO_HURT).get(), 75);
                    fire = _fire.erase(fire);
                    if (fire == _fire.end()) {
                        return;
                    }
                }
            }
        }
        
        // Delete when out of bounds
        auto pos = (*fire)->sprite().position();
        if (pos.x() < -64 || pos.x() > 1100 || pos.y() < -64 || pos.y() > 900) {
            fire = _fire.erase(fire);
            if (fire == _fire.end())
                break;
        }
    }
}

void Room::setPlayer(Player *player) {
    _player = player;
}

Monster* Room::randomMonster() {
    std::vector<Monster*> monsters;
    for (auto* e : _entities) {
        if (e->type() == Entity::Monster)
            monsters.push_back(e->as<Monster>());
    }
    
    return monsters.at(random(0, monsters.size()-1));
}
