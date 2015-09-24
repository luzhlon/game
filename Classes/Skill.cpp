#include "Skill.h"
#include "Soldier.h"
#include "NetRoom.h"

#define PROGRESS_TAG 13

extern Soldier *g_self;

void Skill::cooling() {
    ProgressTimer *prog = static_cast<ProgressTimer *>(_button->getChildByTag(PROGRESS_TAG));
    //判断之前是否已经设置了冷却效果
    if (!prog) {
        prog = ProgressTimer::create(Sprite::create("image/1.png"));
        prog->setType( ProgressTimer::Type::RADIAL);
        prog->setTag(PROGRESS_TAG);
        _button->addChild(prog);
        prog->setAnchorPoint(Vec2(0.f, 0.f));
        prog->setPosition(0.f, 0.f);
        prog->setScale(_button->getContentSize().width / prog->getContentSize().width); //调整到和图标一样大小 
    }

    auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100.f), ProgressTo::create(_cool_time, 0.f));
    prog->runAction(to1);
}

//技能是否正在冷却
bool Skill::is_cooling() {
    auto prog = _button->getChildByTag(PROGRESS_TAG);
    return (prog && (prog->getNumberOfRunningActions() > 0));
}

