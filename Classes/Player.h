#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "WomanSoldier.h"
#include "Skill.h"

class Player {
public:
    static Soldier::Type s_type;
    static Player *getInstance(Soldier *soldier = nullptr);

    static Skill *skill_kick;
    static Skill *skill_boxing;
    static Skill *skill_special;

    void show_circle(bool show = true); //ÏÔÊ¾Ðý×ªµÄÈ¦È¦
    void draw_circle(float radius, Color4F& color = Color4F(0.f, 1.f, 0.f, 0.4)); //ÔÚ½Åµ×»­È¦È¦,ÓÃÓÚÏÔÊ¾·¶Î§
    inline void draw_clear() { _draw->clear(); }

private:
    Player(Soldier *);
    Soldier *_soldier = nullptr;
    DrawNode *_draw = nullptr;
};

#endif /* __PLAYER_H__ */
