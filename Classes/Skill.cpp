#include "Skill.h"

#define PROGRESS_TAG 13

Ref *Skill::btn_skill_kick;
Ref *Skill::btn_skill_boxing;
Ref *Skill::btn_skill_special;

void Skill::Cooling(Node *btn, float time) {
    ProgressTimer *prog = static_cast<ProgressTimer *>(btn->getChildByTag(PROGRESS_TAG));

    if (!prog) {
        prog = ProgressTimer::create(Sprite::create("image/1.png"));
        prog->setType( ProgressTimer::Type::RADIAL);
        prog->setTag(PROGRESS_TAG);
        btn->addChild(prog);
        prog->setAnchorPoint(Vec2(0.f, 0.f));
        prog->setPosition(0.f, 0.f);
    }

    auto to1 = Sequence::createWithTwoActions(ProgressTo::create(time, 100), ProgressTo::create(0, 0));
    //prog->runAction( RepeatForever::create(to1));
    prog->runAction(to1);
}

void Skill::onSkillClicked(Ref *ref) {
    auto btn = (Button *)ref;
    auto prog = btn->getChildByTag(PROGRESS_TAG);
    //on cooling
    if (prog && (prog->getNumberOfRunningActions() > 0)) return;

    do {
        if (btn_skill_kick == ref) {
            Skill::Cooling(btn, 1.f);
            break;
        }
        if (btn_skill_boxing == ref) {
            Skill::Cooling(btn, 2.f);
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