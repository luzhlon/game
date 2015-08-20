#ifndef __HORSESOLDIER_H__
#define __HORSESOLDIER_H__

#include "Soldier.h"

class HorseSoldier : public Soldier
{
public:
	HorseSoldier();
	~HorseSoldier();

    virtual bool init();

	CREATE_FUNC(HorseSoldier);
};

#endif //__HELLOWORLD_SCENE_H__
