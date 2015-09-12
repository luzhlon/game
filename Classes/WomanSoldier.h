#ifndef __WOMANSOLDIER_H__
#define __WOMANSOLDIER_H__

#include "Soldier.h"

class WomanSoldier : public Soldier
{
public:
    CREATE_FUNC(WomanSoldier);

    bool init_soldier() override;

	void loadAction();

	inline void action_idle() {
		runAction(m_act_idle);
	}
	inline void action_walk() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_walk);
	}
	inline void action_run() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_run);
	}
	void action_throw() {
        addState(SOLDIER_STATE_ACTION);
        _cur_action = m_act_throw;
        _cur_action_pu = _par_line_attack;

		runAction(m_act_throw);
        _cur_action_pu->startParticleSystem();
	}
	inline void action_kick() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_kick);
	}
private:
	Animate3D *m_act_idle;
	Animate3D *m_act_walk;
	Animate3D *m_act_run;
	Animate3D *m_act_throw;
	Animate3D *m_act_kick;

    PUParticleSystem3D *_par_line_attack;
};

#endif // __WOMANSOLDIER_H__
