#pragma once

#include <JEngine/JEngine.h>
#include <JEngine/Graphics/Sprite.h>
#include <JEngine/Util/Ref.h>

#include "Common.h"

using namespace JEngine;

class Entity
{
public:
    enum Type {
        Generic,
        Player,
        Door,
        Monster,
        Wall,
        Chest
    };

protected:
    Ref<Sprite> _sprite;
    Vector2i _pos;
    Type _type;
    
public:
    Entity(Ref<Sprite> sprite, Type type);
    virtual ~Entity();
    
    bool intersects(const Entity& other);
    bool adjacentTo(const Entity& other);
    
    virtual void onTouch(Entity* other) {}
    
    Sprite& sprite() { return *_sprite; }
    int x() const { return _pos.x(); }
    int y() const { return _pos.y(); }
    Type type() const { return _type; }
    
    template <typename T>
    T* as();
    
    virtual void translate(Vector2f dv);
    void setPosition(Vector2i v, bool animation_makeup = false);
};

template<typename T>
T *Entity::as() {
    return dynamic_cast<T*>(this);
}
