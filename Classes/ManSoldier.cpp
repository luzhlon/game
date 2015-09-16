#include "ManSoldier.h"

bool ManSoldier::init_soldier() {
    initWithFile("man/man.c3b");
    setRotation3D(Vec3(180.f, 180.f, 180.f));
    setScale(0.1f);
    _height_offset = 0.f;
    _name = "Man";

    return true;
}
