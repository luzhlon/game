#include "WomanSoldier.h"
#include "AppDelegate.h"

bool WomanSoldier::init_soldier() {
    initWithFile("girl/girl.c3b");
    setRotation3D(Vec3(180.f, 180.f, 180.f));
    setScale(0.2f);
    _height_offset = 0.f;
    _name = "Woman";

	loadAction();

    return true;
}

//#include "Particle3D/PU/CCPUListener.h"

void WomanSoldier::loadAction() {
	m_act_idle = Animate3D::createWithFrames(Animation3D::create("girl/idle.c3b"), 0, 100);
	m_act_idle->retain();
	m_act_walk = Animate3D::createWithFrames(Animation3D::create("girl/walk.c3b"), 0, 100);
	m_act_walk->retain();
	m_act_run = Animate3D::createWithFrames(Animation3D::create("girl/run.c3b"), 0, 100);
	m_act_run->retain();
	m_act_throw = Animate3D::createWithFrames(Animation3D::create("girl/throw.c3b"), 0, 100);
	m_act_throw->retain();
	m_act_kick = Animate3D::createWithFrames(Animation3D::create("girl/kick.c3b"), 0, 100);
	m_act_kick->retain();

    _par_line_attack = PUParticleSystem3D::create("line_attack.pu");
    CC_ASSERT(_par_line_attack);
    addChild(_par_line_attack);
}
