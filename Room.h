#pragma once

#include <vector>
#include <functional>
#include "Entity.h"
#include "Door.h"
#include "Fire.h"
#include "Player.h"
#include "Monster.h"

class Room
{
private:
    std::vector<Entity*> _entities;
    std::vector<Fire*> _fire;
    Room* _next;
    Door* _door;
    bool _is_last;
    Player* _player;
    bool _paused;
    Ref<Timer> _blue_fire;
    Spritesheet& _spritesheet;
    int _level;
    
public:
    std::vector<Ref<Sound>> g_sounds;
    Room(Spritesheet& spritesheet, int level, Room* next = nullptr);
    ~Room();
    
    void setPlayer(Player* player);
    void setNextRoom(Room* next);
    void setDoor(Door* door);
    void setIsLast(bool state);
    void addEntity(Entity* entity);
    void forEachEntitiy(std::function<void(Entity*)> fn);
    void setPaused(bool state) { _paused = state; }
    
    void createFire(Ref<Sprite> sprite, Vector2f dv, Fire::FireType type = Fire::Regular);
    void renderFire(Matrix4f view);
    void moveFire(float delta);
    Monster* randomMonster();
    
    Room* nextRoom() const { return _next; }
    Door* door() const { return _door; }
    bool isLast() const { return _is_last; }
};