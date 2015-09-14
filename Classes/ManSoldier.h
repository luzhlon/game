#ifndef __MANSOLDIER_H__
#define __MANSOLDIER_H__

#include "Soldier.h"

class ManSoldier : public Soldier {
public:
    bool init_soldier() override;

    CREATE_FUNC(ManSoldier);

};

#endif // __MANSOLDIER_H__
