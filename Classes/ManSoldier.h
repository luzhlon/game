#ifndef __MANSOLDIER_H__
#define __MANSOLDIER_H__

#include "Soldier.h"

class ManSoldier : public Soldier {
public:
    CREATE_FUNC(ManSoldier);
    bool init_soldier() override;
};

#endif // __MANSOLDIER_H__
