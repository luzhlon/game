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

    float _blood = 0.f; // 对血量的影响
    float _delay_time = 0.f; // 伤害延时
    Type _type = (Type)(-1);
};

struct Skill : public SkillBase {
    float _distance = 0.f; // 造成伤害的距离范围
    float _angle = 45.f; // 造成伤害的角度范围
    float _cool_time = 0.f;  // 冷却时间
    float _magic_dec = 0.f; // 魔力值消耗
    void cooling(); // 冷却
    bool is_cooling(); // 是否正在冷却


    Button *_button = nullptr;
};

#endif /* __SKILL_H__ */