#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "WomanSoldier.h"

class Player {
public:
    static Soldier::Type s_type;
    static Player *getInstance(Soldier *soldier = nullptr);

    void showCircle(bool show = true); //��ʾ��ת��ȦȦ
    void draw_circle(float radius, Color4F& color = Color4F(0.f, 1.f, 0.f, 0.4)); //�ڽŵ׻�ȦȦ,������ʾ��Χ
    inline void draw_clear() { _draw->clear(); }

private:
    Player(Soldier *);
    Soldier *_soldier = nullptr;
    DrawNode *_draw = nullptr;
};


#endif /* __PLAYER_H__ */
