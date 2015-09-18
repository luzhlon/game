#include "WomanSoldier.h"
#include "AppDelegate.h"

bool WomanSoldier::init_soldier() {
    initWithFile("girl/girl.c3b");
    setScale(0.1f);
    _height_offset = 0.f;
    _name = "Woman";
    _role_id = SOLDIER_TYPE_WOMAN;

	load_actions();

    return true;
}

//#include "Particle3D/PU/CCPUListener.h"

void WomanSoldier::load_actions() {
    _par_line_attack = PUParticleSystem3D::create("line_attack.pu");
    CC_ASSERT(_par_line_attack);
    addChild(_par_line_attack);

    m_act_idle = Animate3D::createWithFrames(Animation3D::create("girl/idle.c3b"), 0, 500);
    m_act_walk = Animate3D::createWithFrames(Animation3D::create("girl/walk.c3b"), 0, 30);
    m_act_kick = Animate3D::createWithFrames(Animation3D::create("girl/kick.c3b"), 0, 119);
    m_act_boxing = Animate3D::createWithFrames(Animation3D::create("girl/boxing.c3b"), 0, 119);
    m_act_special = Animate3D::createWithFrames(Animation3D::create("girl/special.c3b"), 0, 80);
}
