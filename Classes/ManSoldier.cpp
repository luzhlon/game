#include "ManSoldier.h"

bool ManSoldier::init_soldier() {
    initWithFile("man/man.c3b");
    setRotation3D(Vec3(180.f, 180.f, 180.f));
    setScale(0.1f);
    _height_offset = 0.f;
    _name = "Man";
    _role_id = SOLDIER_TYPE_MAN;

    load_actions();

    return true;
}

void ManSoldier::load_actions() {
    m_act_idle = Animate3D::createWithFrames(Animation3D::create("man/idle.c3b"), 0, 1000);
    m_act_walk = Animate3D::createWithFrames(Animation3D::create("man/walk.c3b"), 0, 30);
    m_act_kick = Animate3D::createWithFrames(Animation3D::create("man/kick.c3b"), 0, 119);
    m_act_boxing = Animate3D::createWithFrames(Animation3D::create("man/boxing.c3b"), 0, 87);
    m_act_special = Animate3D::createWithFrames(Animation3D::create("man/special.c3b"), 0, 100);
}
