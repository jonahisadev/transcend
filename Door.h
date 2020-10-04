#pragma once

#include "Entity.h"

class Door : public Entity
{
public:
    Door(Ref<Sprite> sprite) : Entity(sprite, Type::Door) {}
    virtual ~Door() {}
};