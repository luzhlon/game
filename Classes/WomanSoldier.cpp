#include "WomanSoldier.h"
#include "AppDelegate.h"

bool WomanSoldier::init_soldier() {
    initWithFile("girl/girl.c3b");
    setScale(0.7f);
    _height_offset = 0.f;
    _name = "Woman";

	loadAction();

    return true;
}

//#include "Particle3D/PU/CCPUListener.h"

void WomanSoldier::loadAction() {
    _par_line_attack = PUParticleSystem3D::create("line_attack.pu");
    CC_ASSERT(_par_line_attack);
    addChild(_par_line_attack);
}

void WomanSoldier::action_boxing() {
    Soldier::action_boxing();
    _cur_action_pu = _par_line_attack;
    //_cur_action_pu->startParticleSystem();
}
