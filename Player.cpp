#include "Player.h"
#include "Door.h"
#include "ChestKey.h"

#include <iostream>

Player::Player(Ref<Sprite> sprite, Ref<Sprite> aim)
    : Entity(sprite, Type::Player), _should_move(false), _aim(aim), _item(nullptr)
{
    _health = 5;
    _magic = 5;
    _magic_callback = [&, this]() {
        if (_magic < 5)
            _magic++;
    };
    _magic_timer = make_ref<Timer>(3000, _magic_callback, true);
}

void Player::onTouch(Entity *other)
{
    if (other->type() == Type::Door)
        _should_move = true;
    else if (other->type() == Type::Chest) {
        auto* chest = other->as<::Chest>();
        if (item() && item()->type() == Chestable::ItemKey) {
            auto* key = item()->as<ChestKey>();
            if (key->id() == chest->key()->id()) {
                chest->open();
                setItem(chest->item());
                item()->sprite().setSize({64, 64});
                Audio::playSound(g_sounds.at(SO_CHEST).get(), 75);
            }
        }
    }
}

void Player::setItem(Chestable *item) {
    _item = item;
}
