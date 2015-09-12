#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "WomanSoldier.h"

class Player {
public:
    static Soldier::Type s_type;
    static Player *getInstance(Soldier *soldier = nullptr);

    void showCircle(bool show = true);

private:
    Player(Soldier *);
    Soldier *_soldier = nullptr;
};


#endif /* __PLAYER_H__ */
