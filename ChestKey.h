#pragma once

#include <JEngine/JEngine.h>
#include <JEngine/Graphics/Sprite.h>
#include <JEngine/Util/Ref.h>

#include "Chest.h"

class ChestKey : public Chestable {
private:
    int _id;
    
public:
    ChestKey(Ref<Sprite> sprite, int id) : Chestable(sprite, Type::ItemKey), _id(id) {}

    virtual ~ChestKey() {}

    void setColor(int r, int g, int b);

    JEngine::Sprite &sprite() { return *_sprite; }
    int id() const { return _id; }
};