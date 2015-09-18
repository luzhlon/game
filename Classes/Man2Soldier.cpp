#include "Man2Soldier.h"

bool Man2Soldier::init_soldier() {
    initWithFile("man2/man2.c3b");
    setScale(0.1f);
    _height_offset = 0.f;
    _name = "Man2";
    _role_id = SOLDIER_TYPE_MAN2;

    load_actions();

    return true;
}

void Man2Soldier::load_actions() {
    m_act_idle = Animate3D::createWithFrames(Animation3D::create("man2/idle.c3b"), 0, 1000);
    m_act_walk = Animate3D::createWithFrames(Animation3D::create("man2/walk.c3b"), 0, 30);
    m_act_kick = Animate3D::createWithFrames(Animation3D::create("man2/kick.c3b"), 0, 119);
    m_act_boxing = Animate3D::createWithFrames(Animation3D::create("man2/boxing.c3b"), 0, 87);
    m_act_special = Animate3D::createWithFrames(Animation3D::create("man2/special.c3b"), 0, 100);
}
