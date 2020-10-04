#pragma once

#include "Entity.h"

class ChestKey;
struct Chestable
{
public:
    enum Type {
        ItemKey,
        ItemAmulet
    };
    
protected:
    Ref<Sprite> _sprite;
    Type _type;
    
public:
    Chestable(Ref<Sprite> sprite, Type type) : _sprite(sprite), _type(type) {}
    virtual ~Chestable() {}
    
    Type type() const { return _type; }
    
    template <typename T>
    T* as();
    
    Sprite& sprite() { return *_sprite; }
};

class Chest : public Entity
{
private:
    ChestKey* _key;
    Chestable* _item;
    
public:
    Chest(Ref<Sprite> sprite, ChestKey* key);
    virtual ~Chest() {}
    
    void setItem(Chestable* item);
    Chestable* item() { return _item; }
    ChestKey* key() { return _key; }
    
    void open();
};

template<typename T>
T *Chestable::as() {
    return dynamic_cast<T*>(this);
}
