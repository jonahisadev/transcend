#pragma once

#include <JEngine/JEngine.h>
#include <JEngine/Graphics/Sprite.h>
#include <JEngine/Util/Ref.h>

using namespace JEngine;

class Fire
{
public:
    enum FireType
    {
        Regular,
        Blue
    };
    
private:
    Ref<Sprite> _sprite;
    Vector2f _dv;
    FireType _fire_type;
    
public:
    Fire(Ref<Sprite> sprite, Vector2f dv, FireType firetype) : _sprite(sprite), _dv(dv), _fire_type(firetype) {}
    virtual ~Fire() {}
    
    Sprite& sprite() { return *_sprite; }
    const Vector2f& dv() { return _dv; }
    
    void setFireType(FireType fire_type);
    FireType fireType() const { return _fire_type; }
};