#ifndef __WOMANSOLDIER_H__
#define __WOMANSOLDIER_H__

#include "Soldier.h"

class WomanSoldier : public Soldier
{
public:
    CREATE_FUNC(WomanSoldier);

    virtual bool init();
};

#endif // __WOMANSOLDIER_H__
