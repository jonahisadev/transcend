#include "Monster.h"

void Monster::addKey(ChestKey *key)
{
    _key = key;
}

void Monster::translate(Vector2f dv) {
    Entity::translate(dv);
    if (_key)
        _key->sprite().translate(dv);
}
