#include "Skill.h"
#include "Soldier.h"

#define PROGRESS_TAG 13

Ref *Skill::btn_skill_kick;
Ref *Skill::btn_skill_boxing;
Ref *Skill::btn_skill_special;

extern Soldier *g_self;

void Skill::Cooling(Node *btn, float time) {
    ProgressTimer *prog = static_cast<ProgressTimer *>(btn->getChildByTag(PROGRESS_TAG));
    //判断之前是否已经设置了冷却效果
    if (!prog) {
        prog = ProgressTimer::create(Sprite::create("image/1.png"));
        prog->setType( ProgressTimer::Type::RADIAL);
        prog->setTag(PROGRESS_TAG);
        btn->addChild(prog);
        prog->setAnchorPoint(Vec2(0.f, 0.f));
        prog->setPosition(0.f, 0.f);
        prog->setScale(btn->getContentSize().width / prog->getContentSize().width); //调整到和图标一样大小 
    }

    auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100.f), ProgressTo::create(time, 0.f));
    prog->runAction(to1);
}

void Skill::onSkillClicked(Ref *ref) {
    auto btn = (Button *)ref;
    auto prog = btn->getChildByTag(PROGRESS_TAG);
    //技能正在冷却
    if (prog && (prog->getNumberOfRunningActions() > 0)) return;

    do {
        if (btn_skill_kick == ref) {
            Skill::Cooling(btn, 1.f);
            g_self->action_kick();
            break;
        }
        if (btn_skill_boxing == ref) {
            Skill::Cooling(btn, 2.f);
            g_self->action_boxing();
            break;
        }
        if (btn_skill_special == ref) {
            Skill::Cooling(btn, 3.f);
            break;
        }
        if (btn_skill_kick == ref) {
            Skill::Cooling(btn, 1.f);
            break;
        }
    } while (false);
};