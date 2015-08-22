#ifndef __MANSOLDIER_H__
#define __MANSOLDIER_H__

#include "Soldier.h"

class ManSoldier : public Soldier {
public:
    virtual bool init();

    CREATE_FUNC(ManSoldier);

};

#endif // __MANSOLDIER_H__
