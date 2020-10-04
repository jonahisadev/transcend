#include <JEngine/JEngine.h>
#include <JEngine/Game/BaseGame.h>
#include <JEngine/Graphics/BasicQuad.h>
#include <JEngine/Graphics/Spritesheet.h>
#include <JEngine/Graphics/Sprite.h>
#include <JEngine/Graphics/Font.h>
#include <JEngine/Util/Ref.h>
#include <JEngine/Audio/Audio.h>

#include <cstdlib>
#include <ctime>

#include "Common.h"
#include "Entity.h"
#include "Player.h"
#include "Door.h"
#include "Room.h"
#include "Monster.h"
#include "Wall.h"
#include "Chest.h"
#include "Amulet.h"

using namespace JEngine;

class Game : public BaseGame
{
public:
    enum {
        Playing,
        Dead
    } state = Playing;
    
private:
    Ref<TexturedQuad> background;
    Ref<TexturedQuad> dead_screen;
    
    Ref<Spritesheet> spritesheet;
    Ref<Player> player;
    std::function<void(Vector2f)> player_move;
    std::function<void(Vector2f)> monster_move;
    Room* current_room;
    
    Ref<Font> font;
    bool debug;
    
    Ref<Sprite> heart;
    Ref<Sprite> empty_heart;
    Ref<Sprite> fire;
    Ref<Sprite> empty_fire;
    
    int level;
    
    std::vector<Ref<Sound>> g_sounds;
    Ref<Sound> soundtrack;
    
public:
    Game() : BaseGame(1024, 704, "Transcend - LD47"), debug(false)
    {
        flags() << EnableFonts << EnableAudio;
        #ifdef RELEASE_MODE
            setResourceLocation("res");
        #else
            setResourceLocation("../res");
        #endif
        
        window().vsync(true);
        window().center();
        
        spritesheet = make_ref<Spritesheet>(0, 0, 0, 0, res("spritesheet.png"), 16);
        
        auto player_sprite = spritesheet->createSprite(64, 64, 64, 64, S_PLAYER);
        player = make_ref<Player>(player_sprite, spritesheet->createSprite(128, 64, 64, 64, S_AIM));
        
        player_move = [&](Vector2f dv)
        {
            player->translate(dv);
        };
        
        font = make_ref<Font>(res("VT323-Regular.ttf"), 32, &window());
        
        background = make_ref<TexturedQuad>(0, 0, 1024, 704, res("background.png"));
        dead_screen = make_ref<TexturedQuad>(0, 0, 1024, 704, res("dead.png"));
        
        heart = spritesheet->createSprite(0, 0, 32, 32, S_HEART);
        empty_heart = spritesheet->createSprite(0, 0, 32, 32, S_EMPTY_HEART);
        fire = spritesheet->createSprite(0, 0, 32, 32, S_FIRE);
        empty_fire = spritesheet->createSprite(0, 0, 32, 32, S_EMPTY_FIRE);
        
        g_sounds.push_back(make_ref<Sound>(res("audio/chest.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/dead.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/door.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/drop.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/fire.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/hurt.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/win.wav")));
        g_sounds.push_back(make_ref<Sound>(res("audio/kill.wav")));
        player->g_sounds = g_sounds;
        
        soundtrack = make_ref<Sound>(res("audio/transcend.mp3"));
        Audio::playSound(soundtrack.get(), 35, true);
    }
    virtual ~Game() {}
    
    void createLevel(int n)
    {
        level = n;
        int num_rooms = (n / 2) + 3;
        
        Room* prev = nullptr;
        Room* current = nullptr;
        Room* first = nullptr;
        
        ChestKey* first_key = nullptr;
        
        // Generate keys and chests
        int num_keys = num_rooms-1;
        std::vector<ChestKey*> keys;
        std::vector<Chest*> chests;
        for (int i = 0; i < num_keys; i++) {
            int r = random(0, 255);
            int g = random(0, 255);
            int b = random(0, 255);
            
            auto* key = new ChestKey(spritesheet->createSprite(0, 0, 32, 32, S_KEY), i);
            key->setColor(r, g, b);
            auto* chest = new Chest(spritesheet->createSprite(0, 0, 64, 64, S_CHEST), key);
            keys.push_back(key);
            chests.push_back(chest);
        }
        
        // Match up keys and chests
        int index = 1;
        for (int i = 1; i < num_keys; i++) {
            auto chest = chests.at(i-1);
            auto* key = keys.at(i);
            chest->setItem(key);
        }
        chests.back()->setItem(new Amulet(spritesheet->createSprite(0, 640, 64, 64, S_AMULET)));
        first_key = keys.at(0);
        
        // Generate rooms
        for (int i = 0; i < num_rooms; i++) {
            // Room
            current = new Room(*spritesheet, n);
            current->g_sounds = g_sounds;
            current->setPlayer(player.get());
            
            // Door
            float door_y = (float)random(1, 9);
            auto* door = new Door(spritesheet->createSprite(960, door_y * 64, 64, 64, S_DOOR));
            current->addEntity(door);
            current->setDoor(door);

            // Generate monsters
            int num_monsters = std::rand() % (num_rooms-1) + 2;
            for (int i = 0; i < num_monsters; i++) {
                auto rand_x = (float)random(3, 14);
                auto rand_y = (float)random(3, 8);
                auto* monster = new Monster(spritesheet->createSprite(rand_x * 64, rand_y * 64, 64, 64, S_MONSTER));
                current->addEntity(monster);
            }
            
            // Walls
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 1024; j += 64) {
                    current->addEntity(new Wall(spritesheet->createSprite(j, i * 640, 64, 64, S_WALL)));
                }
                for (int j = 64; j < 640; j += 64) {
                    auto* wall = new Wall(spritesheet->createSprite(i * 960, j, 64, 64, S_WALL));
                    if (wall->intersects(*door)) {
                        delete wall;
                        continue;
                    }
                    current->addEntity(wall);
                }
            }
            
            // Chests
            if (i != 0) {
                int loc_id = random(0, 2);
                Vector2f location = {0,0};
                switch (loc_id) {
                    case 0:
                        location = {896, 64};
                        break;
                    case 1:
                        location = {896, 576};
                        break;
                    case 2:
                        location = {64, 576};
                        break;
                    default:
                        break;
                }
                
                int idx = random(0, chests.size()-1);
                auto chest = chests.at(idx);
                chest->sprite().setPosition(location);
                chest->setPosition({(int)location.x() / 64, (int)location.y() / 64});
                current->addEntity(chest);
                chests.erase(chests.begin() + idx);
            }
            
            // Room math
            if (prev == nullptr)
                first = current;
            else
                prev->setNextRoom(current);
            prev = current;
        }
        
        // More room math
        current->setNextRoom(first);
        current->setIsLast(true);
        current->randomMonster()->addKey(first_key);
        
        current_room = first;
        current_room->setPaused(false);
        
        // Reset player health
        player->setHealth(5);
    }
    
    void moveMonsters()
    {
        current_room->forEachEntitiy([&](Entity* e) {
            if (e->type() == Entity::Monster) {
                auto fn = [&, e](Vector2f dv) {
                    e->translate(dv);
                };
                
                auto dir = rand() % 4;
                switch (dir) {
                    case 0:
                        if (e->x() < 14)
                            Animate::linear({0, 0}, {64, 0}, 200, Animation::Relative, fn);
                        break;
                    case 1:
                        if (e->x() > 1)
                            Animate::linear({0, 0}, {-64, 0}, 200, Animation::Relative, fn);
                        break;
                    case 2:
                        if (e->y() < 9)
                            Animate::linear({0, 0}, {0, 64}, 200, Animation::Relative, fn);
                        break;
                    case 3:
                        if (e->y() > 1)
                            Animate::linear({0, 0}, {0, -64}, 200, Animation::Relative, fn);
                        break;
                    default:
                        break;
                }
            }
        });
    }
    
    void render(Matrix4f view) override
    {
        // Background
        window().clear(0, 0, 0);
        background->render(view);
        
        // Room entities
        current_room->forEachEntitiy([&](Entity* e) {
            e->sprite().render(view); 
            if (e->type() == Entity::Monster) {
                auto* monster = e->as<Monster>();
                if (monster->key()) {
                    auto pos = monster->sprite().position();
                    pos.translate(0, 16);
                    monster->key()->sprite().setPosition(pos);
                    monster->key()->sprite().render(view);
                }
            }
        });
        current_room->renderFire(view);
        
        // Player + AIM
        player->sprite().render(view);
        player->aim().render(view);
        
        // Player item
        if (player->item()) {
            player->item()->sprite().setPosition(0, 640);
            player->item()->sprite().render(view);
        }
        
        // HUD - Hearts
        int hearts = player->health();
        int h_index;
        for (h_index = 0; h_index < hearts; h_index++) {
            heart->setPosition((h_index * 32) + 16, 16);
            heart->render(view);
        }
        for (; h_index < 5; h_index++) {
            empty_heart->setPosition((h_index * 32) + 16, 16);
            empty_heart->render(view);
        }

        // HUD - Fire
        int fires = player->magic();
        int f_index;
        for (f_index = 0; f_index < fires; f_index++) {
            fire->setPosition((f_index * 32) + 848, 16);
            fire->render(view);
        }
        for (; f_index < 5; f_index++) {
            empty_fire->setPosition((f_index * 32) + 848, 16);
            empty_fire->render(view);
        }
        
        // Dead
        if (state == Dead)
            dead_screen->render(view);

        // HUD - Level
        std::string lvl = "Level " + std::to_string(level);
        font->render((window().width() - font->width(lvl))/2, font->height(lvl) + 32 - (font->height(lvl)/2), lvl, {1, 1, 1});
        
        // Debug
        if (debug) {
            std::string fps = "FPS: " + std::to_string(window().getFPS());
            font->render(5, font->height(fps) + 5, fps, {1, 1, 1});
        }
    }
    
    void update(float delta) override
    {
        if (window().keyOnce('D')) {
            if (player->x() < 14 || player->y() == current_room->door()->y()) {
                Animate::linear({0, 0}, {64, 0}, 200, Animation::Relative, player_move);
                moveMonsters();
            }
        }
        if (window().keyOnce('A') && player->x() > 1) {
            Animate::linear({0, 0}, {-64, 0}, 200, Animation::Relative, player_move);
            moveMonsters();
        }
        if (window().keyOnce('S') && player->y() < 9) {
            Animate::linear({0, 0}, {0, 64}, 200, Animation::Relative, player_move);
            moveMonsters();
        }
        if (window().keyOnce('W') && player->y() > 1) {
            Animate::linear({0, 0}, {0, -64}, 200, Animation::Relative, player_move);
            moveMonsters();
        }
        
        if (window().keyOnce('M'))
            soundtrack->togglePlayback();
        
        if (window().keyOnce(KeyF3))
            debug = !debug;

        current_room->forEachEntitiy([&](Entity* e) {
            if (player.get() == e)
                return;
            
            if (player->intersects(*e))
                player->onTouch(e);
        });
        
        // Next room or level
        if (player->shouldMove()) {
            new Timer(100, [&]() {
                player->sprite().setPosition(64, 64); 
            });
            if (current_room->isLast() && player->item() && player->item()->type() == Chestable::ItemAmulet) {
                createLevel(level+1);
                player->setItem(nullptr);
                Audio::playSound(g_sounds.at(SO_WIN).get(), 75);
            } else
                Audio::playSound(g_sounds.at(SO_DOOR).get(), 75);
            player->setPosition({1, 1}, true);
            current_room->setPaused(true);
            current_room = current_room->nextRoom();
            current_room->setPaused(false);
            player->setShouldMove(false);
        }
        
        // Aim
        auto mouse = window().mousePosition();
        auto pos = player->sprite().position();
        pos = {pos.x() + 32, pos.y() + 32};
        auto rel = mouse - pos;
        if (std::abs(rel.x()) > std::abs(rel.y())) {
            if (rel.x() > 0)
                player->aim().setPosition({pos.x() + 32, pos.y() - 32});
            else
                player->aim().setPosition({pos.x() - 96, pos.y() - 32});
        } else {
            if (rel.y() > 0)
                player->aim().setPosition({pos.x() - 32, pos.y() + 32});
            else
                player->aim().setPosition({pos.x() - 32, pos.y() - 96});
        }
        
        // Make fire
        if (window().mousePressedOnce(Mouse::MouseLeft) && player->magic() > 0) {
            moveMonsters();
            player->lowerMagic();
            Audio::playSound(g_sounds.at(SO_FIRE).get(), 75);
            
            Vector2f dv = {0,0};
            if (player->aim().x() > player->sprite().x())
                dv.setX(FIRE_SPEED);
            else if (player->aim().x() < player->sprite().x())
                dv.setX(-FIRE_SPEED);
            else if (player->aim().y() > player->sprite().y())
                dv.setY(FIRE_SPEED);
            else if (player->aim().y() < player->sprite().y())
                dv.setY(-FIRE_SPEED);
            auto start = player->aim().position();
            current_room->createFire(spritesheet->createSprite(start.x(), start.y(), 64, 64, S_FIRE), dv);
        }
        
        // Move fire
        current_room->moveFire(delta);
        
        // Check if player is dead
        if (player->health() == 0 && state != Dead) {
            state = Dead;
            Audio::playSound(g_sounds.at(SO_DEAD).get(), 75);
        }
    }
};

int main()
{
    Game game;
    game.createLevel(1);
    game.start();
    return 0;
}