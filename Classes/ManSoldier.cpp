#include "ManSoldier.h"

bool ManSoldier::init() {
    if(!Soldier::init()) return false;

    _sprite = Sprite3D::create("test.c3b");
	addChild(_sprite);
    return true;
}
