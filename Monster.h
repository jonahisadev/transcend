#pragma once

#include "Entity.h"
#include "ChestKey.h"

class Monster : public Entity
{
private:
    ::ChestKey* _key;
    
public:
    Monster(Ref<Sprite> sprite) : Entity(sprite, Type::Monster), _key(nullptr) {}
    virtual ~Monster() {}
    
    void addKey(ChestKey* key);
    ChestKey* key() { return _key; }
    
    void translate(Vector2f dv) override;
};