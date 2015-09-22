#ifndef __SKILL_H__
#define __SKILL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

struct SkillBase {
public:
    enum Type {
        SKILL_BOXING = 0,
        SKILL_KICK,
        SKILL_SPECIAL,
        SKILL_SPEED,

        NUMBER_SKILL
    };

    float _blood = 0.f; // ��Ѫ����Ӱ��
    float _delay_time = 0.f; // �˺���ʱ
    Type _type = (Type)(-1);
};

struct Skill : public SkillBase {
    float _distance = 0.f; // ����˺��ľ��뷶Χ
    float _angle = 45.f; // ����˺��ĽǶȷ�Χ
    float _cool_time = 0.f;  // ��ȴʱ��
    float _magic_dec = 0.f; // ħ��ֵ����
    void cooling(); // ��ȴ
    bool is_cooling(); // �Ƿ�������ȴ


    Button *_button = nullptr;
};

#endif /* __SKILL_H__ */