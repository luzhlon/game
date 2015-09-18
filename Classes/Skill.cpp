#include "Skill.h"
#include "Soldier.h"
#include "NetRoom.h"

#define PROGRESS_TAG 13

extern Soldier *g_self;

void SkillUi::cooling(float time) {
    ProgressTimer *prog = static_cast<ProgressTimer *>(_button->getChildByTag(PROGRESS_TAG));
    //�ж�֮ǰ�Ƿ��Ѿ���������ȴЧ��
    if (!prog) {
        prog = ProgressTimer::create(Sprite::create("image/1.png"));
        prog->setType( ProgressTimer::Type::RADIAL);
        prog->setTag(PROGRESS_TAG);
        _button->addChild(prog);
        prog->setAnchorPoint(Vec2(0.f, 0.f));
        prog->setPosition(0.f, 0.f);
        prog->setScale(_button->getContentSize().width / prog->getContentSize().width); //��������ͼ��һ����С 
    }

    auto to1 = Sequence::createWithTwoActions(ProgressTo::create(0, 100.f), ProgressTo::create(time, 0.f));
    prog->runAction(to1);
}

//�����Ƿ�������ȴ
bool SkillUi::cooling() {
    auto prog = _button->getChildByTag(PROGRESS_TAG);
    return (prog && (prog->getNumberOfRunningActions() > 0));
}

void SkillBoxing::onSkillClicked(Ref *ref) {
    //����������ȴ
    if (cooling()) return;
    cooling(_cool_time);
    NetRoom::do_skill(this);
};

void SkillKick::onSkillClicked(Ref *ref) {
    //����������ȴ
    if (cooling()) return;
    cooling(_cool_time);
    NetRoom::do_skill(this);
};

void SkillSpecial::onSkillClicked(Ref *ref) {
    //����������ȴ
    if (cooling()) return;
    cooling(_cool_time);
    NetRoom::do_skill(this);
};