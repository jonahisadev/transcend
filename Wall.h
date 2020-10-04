#pragma once

#include "Entity.h"

class Wall : public Entity 
{
public:
    Wall(Ref<Sprite> sprite) : Entity(sprite, Type::Wall) {}
    virtual ~Wall() {}
};