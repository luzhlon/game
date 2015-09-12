#ifndef __SKILL_H__
#define __SKILL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class Skill {
public:
    static void Cooling(Node *btn, float time);
    static void onSkillClicked(Ref *ref);

    static Ref *btn_skill_kick;
    static Ref *btn_skill_boxing;
    static Ref *btn_skill_special;
};

#endif /* __SKILL_H__ */