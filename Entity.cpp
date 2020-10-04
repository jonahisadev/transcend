#include "Entity.h"
#include <JEngine/Util/Timer.h>

Entity::Entity(Ref<Sprite> sprite, Type type) 
    : _sprite(sprite), _pos(_sprite->x() / CELL, _sprite->y() / CELL), _type(type)
{}

Entity::~Entity()
{}

bool Entity::intersects(const Entity &other) {
    return (x() == other.x() && y() == other.y());
}

bool Entity::adjacentTo(const Entity &other) {
    return false;
}

void Entity::translate(Vector2f dv)
{
    _sprite->translate(dv);
    _pos = {(int)std::round(_sprite->x() / CELL), (int)std::round(_sprite->y() / CELL)};
}

void Entity::setPosition(Vector2i v, bool animation_makeup)
{
    _pos = v;
    _sprite->setPosition({(float)v.x() * 64, (float)v.y() * 64});
    if (animation_makeup)
        _sprite->translate(-32, 0);
}
