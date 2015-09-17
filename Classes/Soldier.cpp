#include "Soldier.h"
#include "World.h"
#include "AppDelegate.h"
#include "ManSoldier.h"
#include "Man2Soldier.h"
#include "WomanSoldier.h"

Vec3 Soldier::s_camera_offset = Vec3(0.f, 60.f, 45.f); 
Soldier *Soldier::s_soldiers[Soldier::Type::TYPE_NUMBER];
Soldier *Soldier::s_followed = nullptr;
const float Soldier::s_step = 10.f;
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

Soldier *Soldier::create(room_member *rm) {
    auto sol = create(rm->m_role_id);
    sol->_room_member = rm;
    sol->begin_fight();
    return sol;
}

bool Soldier::init() {
    //将动作加载放在init子类前面，让子类有修改的机会
    /*
	m_act_idle = Animate3D::createWithFrames(Animation3D::create("idle.c3b"), 0, 100);
	m_act_walk = RepeatForever::create(Animate3D::createWithFrames(Animation3D::create("walk.c3b"), 0, 100));
	m_act_special = Animate3D::createWithFrames(Animation3D::create("run.c3b"), 0, 100);
	m_act_boxing = Animate3D::createWithFrames(Animation3D::create("boxing.c3b"), 0, 119);
	m_act_kick = Animate3D::createWithFrames(Animation3D::create("kick.c3b"), 0, 56); // */

    init_soldier();

    CC_ASSERT(_role_id >= 0);

    m_act_walk = RepeatForever::create(m_act_walk);
	m_act_kick->retain();
	m_act_boxing->retain();
	m_act_special->retain();
	m_act_walk->retain();
	m_act_idle->retain();

    setPosition3D(Vec3::ZERO);

    return true;
}

void Soldier::net_move(Vec3& target) {
}

void Soldier::net_move_stop() {
}

void Soldier::begin_fight() {
    auto node = CSLoader::createNode("soldier_head_info.csb");
    auto layout = static_cast<Layout *>(node->getChildByTag(1));
    auto textName = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_name"));
    _blood_bar = static_cast<LoadingBar *>(Helper::seekWidgetByName(layout, "loadbar_blood"));

    textName->setString(_name);
    textName->setFontName("fonts/arial.ttf");
    textName->setScale(1.f);
    _blood_bar->setPercent(_blood * 100.f / s_full_blood);
    layout->removeFromParent();

    _billboard = BillBoard::create();
    _billboard->addChild(layout);
    layout->setPositionY(10.f);
    _billboard->setPosition3D(Vec3(0.f, 270.f, 0.f));
    addThing(_billboard);

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
        if ((getPosition3D() - _target_point).length() < 1.f) {
            move_stop();
        }
        else {
            updatePosition(dt);
        }
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

    if (World::CAMERA_I != g_world->getCameraMask()) return;
    auto camera = g_world->get_camera();
    if (this == s_followed) {
        camera->setPosition3D(getPosition3D() + s_camera_offset);
        camera->lookAt(getPosition3D());
    }
}

void Soldier::updateRotation() {
    Vec3 rota = getRotation3D();
    auto delta = _target_point - getPosition3D();
    rota.y = CC_RADIANS_TO_DEGREES(Vec2(delta.x, delta.z).getAngle());
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

void Soldier::CameraZoom(float factor) {
    auto v = s_camera_offset;
    v.normalize();
    s_camera_offset += v * factor;
}

void Soldier::CameraRotate(Vec2 &v) {
    //Rotate X-Z coordinate
    if (fabs(v.x) > fabs(v.y)) {
        Vec2 v_xz(s_camera_offset.x, s_camera_offset.z);
        v_xz.rotate(Vec2::ZERO, v.x * 0.02);
        //
        s_camera_offset.x = v_xz.x;
        s_camera_offset.z = v_xz.y;
    }
    else {
        //Rotate Y-Z coordinate
        Vec2 v_yz(s_camera_offset.y, s_camera_offset.z);
        v_yz.rotate(Vec2::ZERO, v.y * 0.02);
        //
        s_camera_offset.y = v_yz.x;
        s_camera_offset.z = v_yz.y;
    }
}

void Soldier::show_blood_decline(float dec) {
    static Text *text = nullptr;
    if (!text) {
        text = Text::create("", "fonts/Marker Felt.ttf", 20);
        text->setCascadeOpacityEnabled(true);
        add2Billboard(text);
    }
    char str[128];
    sprintf(str, "- %g", dec);
    text->setTextColor(Color4B(Color4F(255.f, 0.f, 0.f, 200.f)));
    text->setPosition(Vec2(0.f, 10.f));
    text->setScale(1);
    text->setString(str);
    text->setOpacity(255.f);

    auto move = MoveBy::create(1.5f, Vec2(0, 40.f));
    auto scale = ScaleBy::create(1.5f, 1.3f);
    auto fade = FadeOut::create(1.5f);
    text->runAction(move);
    text->runAction(scale);
    text->runAction(fade);
}

void Soldier::addThing(Node *node) {
    node->setCameraMask(getCameraMask());
    addChild(node);
}

void Soldier::add2Billboard(Node *node) {
    node->setCameraMask(_billboard->getCameraMask());
    _billboard->addChild(node);
}

void Soldier::move_stop() {
    rmState(SOLDIER_STATE_MOVE);
    auto walk = getActionByTag(ACTION_WALK);
    this->stopAction(m_act_walk);
}

void Soldier::move(Vec3& target) {
    _target_point = target;
    if (atState(SOLDIER_STATE_MOVE)) return;
    addState(SOLDIER_STATE_MOVE);
    runAction(m_act_walk);
}
