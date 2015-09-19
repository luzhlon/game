#include "Soldier.h"
#include "Skill.h"
#include "World.h"
#include "AppDelegate.h"
#include "ManSoldier.h"
#include "Man2Soldier.h"
#include "WomanSoldier.h"

Soldier *Soldier::s_followed = nullptr;
const float Soldier::s_full_blood = 100.f;
extern World *g_world;

#define ACTION_MOVE 0
#define ACTION_WALK 1

Soldier *Soldier::create(int type_id) {
    switch (type_id) {
    case SOLDIER_TYPE_WOMAN:
        return WomanSoldier::create();
        break;
    case SOLDIER_TYPE_MAN:
        return ManSoldier::create();
        break;
    case SOLDIER_TYPE_MAN2:
        return Man2Soldier::create();
        break;
    }
}

bool Soldier::do_skill(SkillBase* skill) {
    move_stop();
    switch (skill->_type) {
    case SkillBase::SKILL_BOXING:
        stopAllActions();
        runAction(m_act_boxing);
        break;
    case SkillBase::SKILL_KICK:
        stopAllActions();
        runAction(m_act_kick);
        break;
    case SkillBase::SKILL_SPECIAL:
        stopAllActions();
        runAction(m_act_special);
        break;
    }
    return true;
}

bool Soldier::be_injured(SkillBase *skill) {
    _blood -= skill->_dec_blood;
    set_blood(_blood);
    return true;
}

Soldier *Soldier::create(room_member *rm) {
    auto sol = create(rm->m_role_id);
    sol->_room_member = rm;
    sol->begin_fight();
    return sol;
}

bool Soldier::init() {
    init_soldier();

    CC_ASSERT(_role_id >= 0);

    setRotation3D(Vec3(180.f, _base_angle + 90.f, 180.f));

    m_act_walk = RepeatForever::create(m_act_walk);
    m_act_idle = RepeatForever::create(m_act_idle);

	m_act_kick->retain();
	m_act_boxing->retain();
	m_act_special->retain();
	m_act_walk->retain();
	m_act_idle->retain();

    setPosition3D(Vec3::ZERO);

    return true;
}

bool Soldier::load_config(char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return false;

    ActionInterval **anim[5] = {
        &m_act_walk,
        &m_act_boxing,
        &m_act_kick,
        &m_act_special,
        &m_act_idle
    };

	char buf[1024];
    // 加载人物
    char role_file[256];
    float scale, base_angle;
    while (fgets(buf, 1024, f))
        if (3 == sscanf(buf, "%s%f%f", role_file, &scale, &base_angle))
            break;
    initWithFile(role_file);
    setScale(scale);
    _base_angle = base_angle;
    setRotation3D(Vec3(180.f, _base_angle, 180.f));
    // 加载动作
    char act_file[256]; 
    int start_frame, end_frame;
    float fps;
    for (int i = 0; i < 5; i++) {
        while (fgets(buf, 256, f))
        if (4 == sscanf(buf, "%s%d%d%f", act_file, &start_frame, &end_frame, &fps))
            break;
        *anim[i] = Animate3D::createWithFrames(Animation3D::create(act_file), start_frame, end_frame, fps);
    }
    fclose(f);
	return true;
}

void Soldier::set_blood(float blood) {
    _blood = blood;
    _blood_bar->setPercent(_blood / s_full_blood * 100.f);
}

void Soldier::load_ui() {
    _billboard = BillBoard::create();

    auto node = CSLoader::createNode("soldier_head_info.csb");
    auto layout = static_cast<Layout *>(node->getChildByTag(1));
    layout->removeFromParent();
    layout->setPositionY(10.f);
    _billboard->addChild(layout);

    _billboard->setPosition3D(Vec3(0.f, 270.f, 0.f));
    add_thing(_billboard);

    blood_bar()->setPercent(_blood * 100.f / s_full_blood);
    blood_dec_text()->setVisible(false);
}

void Soldier::begin_fight() {
    load_ui();
    scheduleUpdate();
}

#define AT_STATE(_state) if(atState(_state))

void Soldier::update(float dt) {
    auto world = World::getInstance();
    auto terrain = world->getTerrain();

    do {
    AT_STATE(SOLDIER_STATE_MOVE) {
        updateRotation();
        //判断和目标位置的距离
        if ((getPosition3D() - _target_point).length() < 1.f) { move_stop(); }
        else { updatePosition(dt); }
        //updateHeight();
        break;
    }
    AT_STATE(SOLDIER_STATE_ACTION) {
        if (_cur_action->isDone()) {
            rmState(SOLDIER_STATE_ACTION);
        }
        break;
    }
    } while (false);

    if (this == s_followed) { world->camera_follow(this); }
}

void Soldier::updateRotation() {
    Vec3 rota = getRotation3D();
    auto delta = _target_point - getPosition3D();
    rota.y = CC_RADIANS_TO_DEGREES(Vec2(delta.x, delta.z).getAngle()) + _base_angle;
    setRotation3D(rota);
}

void Soldier::updateHeight() {
    auto playerPos = getPosition3D();

    auto playerModelMat = getParent()->getNodeToWorldTransform();
    playerModelMat.transformPoint(&playerPos);
    Vec3 Normal;
    float player_h = World::getInstance()->getTerrain()->getHeight(playerPos.x, playerPos.z, &Normal);
    //check the player whether is out of the terrain
    if (Normal.isZero()) {
        move_stop();
    }
    else {
        player_h += getHeightOffset();
        playerPos.y = player_h;
        setPosition3D(playerPos);
    }
}

void Soldier::updatePosition(float dt) {
    auto playerPos = getPosition3D();

    auto delta = _target_point - getPosition3D();
    Vec2 angle(delta.x, delta.z);
    angle.normalize();

    playerPos.x += speed() * angle.x * dt;
    playerPos.z += speed() * angle.y * dt;

    //auto playerModelMat = getParent()->getNodeToWorldTransform();
    //playerModelMat.transformPoint(&playerPos);
    Vec3 Normal;
    float player_h = World::getInstance()->getTerrain()->getHeight(playerPos.x, playerPos.z, &Normal);
    //check the player whether is out of the terrain
    if (Normal.isZero()) {
        player_h = playerPos.y;
    }
    else {
        player_h += getHeightOffset();
        playerPos.y = player_h;
        setPosition3D(playerPos);
    }
}

void Soldier::show_blood_decline(float dec) {
    static Vec2 pos_start(0.f, 0.f);

    auto text = blood_dec_text();
    if (pos_start.isZero()) { pos_start = text->getPosition(); }

    char str[128];
    sprintf(str, "%g", dec);

    text->setVisible(true);
    text->setPosition(pos_start);
    text->setScale(1);
    text->setString(str);
    text->setOpacity(255.f);

    auto move = MoveBy::create(1.f, Vec2(0, 100.f));
    auto scale = ScaleBy::create(1.f, 1.5f);
    auto fade = FadeOut::create(1.f);
    text->stopAllActions();
    text->runAction(move);
    text->runAction(scale);
    text->runAction(fade);
}

void Soldier::add_thing(Node *node) {
    node->setCameraMask(getCameraMask());
    addChild(node);
}

void Soldier::add2Billboard(Node *node) {
    node->setCameraMask(_billboard->getCameraMask());
    _billboard->addChild(node);
}

LoadingBar *Soldier::blood_bar() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto bar = static_cast<LoadingBar *>(Helper::seekWidgetByName(layout, "loadbar_blood"));
    CC_ASSERT(bar);
    return bar;
}

Text *Soldier::name_text() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto textName = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_name"));
    CC_ASSERT(textName);
    return textName;
}

Text *Soldier::blood_dec_text() {
    auto layout = static_cast<Layout *>(_billboard->getChildByTag(1));
    auto text = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_dec_blood"));
    CC_ASSERT(text);
    return text;
}

void Soldier::move_stop() {
    switch_state(SOLDIER_STATE_IDLE);
}

void Soldier::move(Vec3& target) {
    switch_state(SOLDIER_STATE_MOVE, &target);
}

void Soldier::switch_state(State state, void *data) {
    switch (state) {
    case Soldier::SOLDIER_STATE_MOVE:
    {
        _target_point = *(Vec3 *)data;
        if (atState(SOLDIER_STATE_MOVE)) break;
        addState(SOLDIER_STATE_MOVE);
        rmState(SOLDIER_STATE_IDLE);
        stopAllActions();
        runAction(m_act_walk);
        break;
    }
    case Soldier::SOLDIER_STATE_IDLE:
    {
        rmState(SOLDIER_STATE_MOVE);
        addState(SOLDIER_STATE_IDLE);
        stopAllActions();
        runAction(m_act_idle);
        break;
    }
    }
}
