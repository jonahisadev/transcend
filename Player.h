#pragma once

#include "Entity.h"
#include "Chest.h"
#include <JEngine/Util/Timer.h>
#include <JEngine/Graphics/BasicQuad.h>

class Player : public Entity
{
private:
    bool _should_move;
    Ref<Sprite> _aim;
    Chestable* _item;
    Ref<Timer> _magic_timer;
    std::function<void()> _magic_callback;
    int _magic, _health;
    Ref<BasicQuad> _hitbox;
    
public:
    std::vector<Ref<Sound>> g_sounds;
    
    Player(Ref<Sprite> sprite, Ref<Sprite> aim);
    virtual ~Player() {}
    
    void onTouch(Entity* other) override;
    
    bool shouldMove() const { return _should_move; }
    void setShouldMove(bool state) { _should_move = state; }
    
    void setItem(Chestable* item);
    Chestable* item() { return _item; }
    
    Sprite& aim() { return *_aim; }
    
    void lowerMagic() { --_magic; }
    void lowerHealth() { --_health; }
    void setHealth(int health) { _health = health; }\
    void centerHitbox();
    
    int magic() const { return _magic; }
    int health() const { return _health; }
    BasicQuad& hitbox() { return *_hitbox; };
};