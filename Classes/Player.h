#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Soldier.h"

class Player : public Node {
public:
    static Soldier::Type s_type;
    static Player *getInstance(Soldier *soldier = nullptr);

    static Skill *skill_kick;
    static Skill *skill_boxing;
    static Skill *skill_special;
    static Skill *skill_speed;

    void show_circle(bool show = true); //��ʾ��ת��ȦȦ
    void draw_circle(float radius, Color4F& color); //�ڽŵ׻�ȦȦ,������ʾ��Χ
    inline void draw_clear() { _draw->clear(); }

    bool do_skill(Skill *skill);

    void update_per_second(float dt);
    void revive(float dt); // ����

    void set_magic(float magic); // ����ħ��ֵ && UI
    inline void add_magic(float magic) { // + 
        set_magic(_magic + magic);
    }
    
    static void onSkillClicked(Ref *ref);

private:
    Player(Soldier *);
    Soldier *_soldier = nullptr;
    DrawNode *_draw = nullptr;

    static const float s_full_magic;
    float _magic = 100.f; // 
};

#endif /* __PLAYER_H__ */
