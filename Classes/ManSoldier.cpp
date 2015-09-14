#include "ManSoldier.h"

bool ManSoldier::init_soldier() {
    initWithFile("girl/girl.c3b");
    //initWithFile("boy/boy.c3b");
    setRotation3D(Vec3(180.f, 180.f, 180.f));
    setScale(0.15f);
    _height_offset = 0.f;
    _name = "Man";

    return true;
}
