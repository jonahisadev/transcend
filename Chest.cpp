//
// Created by Jonah on 10/3/2020.
//

#include "Chest.h"
#include "ChestKey.h"
#include "Common.h"

Chest::Chest(Ref<Sprite> sprite, ChestKey *key)
    : Entity(sprite, Type::Chest), _key(key), _item(nullptr)
{
    _sprite->setColor(_key->sprite().color());
}

void Chest::setItem(Chestable *item)
{
    _item = item;
}

void Chest::open() {
    _sprite->setGridPosition(S_CHEST_OPEN);
}
