#include "HorseSoldier.h"

bool HorseSoldier::init() {
    if(!Soldier::init()) return false;

    _sprite = Sprite3D::create("test.c3b");
    addChild(_sprite);

    return true;
}
