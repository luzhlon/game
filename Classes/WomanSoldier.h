#ifndef __WOMANSOLDIER_H__
#define __WOMANSOLDIER_H__

#include "Soldier.h"

class WomanSoldier : public Soldier
{
public:
    CREATE_FUNC(WomanSoldier);

    virtual bool init();

	void action_walk();
	void action_throw();
private:
	Animation3D *m_anim_walk;
	Animation3D *m_anim_throw;
};

#endif // __WOMANSOLDIER_H__
