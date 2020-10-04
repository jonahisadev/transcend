#pragma once

#include "Chest.h"

class Amulet : public Chestable
{
public:
    Amulet(Ref<Sprite> sprite) : Chestable(sprite, Type::ItemAmulet) {}
    virtual ~Amulet() {}
};