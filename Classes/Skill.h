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

        NUMBER_SKILL
    };
    SkillBase(Type type, float dec_blood, float dec_distance)
        : _type(type), _dec_blood(dec_blood), _dec_distance(dec_distance) {}

    float _dec_blood = 0.f; // ÉËº¦
    float _dec_distance = 0.f; // ¾àÀë
    float _delay_time = 0.f; // ÉËº¦ÑÓÊ±
    Type _type = (Type)(-1);
};

class Skill : public SkillBase {
public:
    Skill(Type type, float dec_blood, float cool_time, float dec_distance)
        : SkillBase(type, dec_blood, dec_distance), _cool_time(cool_time) {}

    float _cool_time = 0.f;  // ÀäÈ´Ê±¼ä
};

class SkillUi : public Skill {
public:
    SkillUi(Type type, float dec_blood, float cool_time, float dec_distance, Button *button)
        : Skill(type, dec_blood, cool_time, dec_distance), _button(button) {
        CC_ASSERT(_button);
    }

    void cooling(float time); // ÀäÈ´
    bool cooling(); // ÊÇ·ñÕýÔÚÀäÈ´

    virtual void onSkillClicked(Ref *ref) {};

    Button *_button;
};

class SkillBoxing : public SkillUi {
public:
    SkillBoxing(Button *button)
        : SkillUi(SKILL_BOXING, 5.f, 3.f, 30.f, button) {
        button->addClickEventListener(CC_CALLBACK_1(SkillBoxing::onSkillClicked, this));
    }
    void onSkillClicked(Ref *ref) override;
};
class SkillKick : public SkillUi {
public:
    SkillKick(Button *button)
        : SkillUi(SKILL_KICK, 8.f, 4.f, 40.f, button) {
        button->addClickEventListener(CC_CALLBACK_1(SkillKick::onSkillClicked, this));
    }
    void onSkillClicked(Ref *ref) override;
};
class SkillSpecial : public SkillUi {
public:
    SkillSpecial(Button *button)
        : SkillUi(SKILL_SPECIAL, 13.f, 5.f, 20.f, button) {
        button->addClickEventListener(CC_CALLBACK_1(SkillSpecial::onSkillClicked, this));
    }
    void onSkillClicked(Ref *ref) override;
};

#endif /* __SKILL_H__ */