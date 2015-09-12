#include "Soldier.h"
#include "World.h"
#include "AppDelegate.h"
#include "WomanSoldier.h"

Soldier *Soldier::s_soldiers[Soldier::SoldierNumber];
Soldier *Soldier::s_followed = nullptr;
const float Soldier::s_full_blood = 100.f;
extern World *g_world;

bool Soldier::loadAllSoldier() {
    bool loaded = true;
	//loaded = loaded && (s_soldiers[0] = HorseSoldier::create());
    loaded = loaded && (s_soldiers[1] = WomanSoldier::create());
	//loaded = loaded && (s_soldiers[2] = ManSoldier::create());
    if(loaded) {
        return true;
    } else {
        return false;
    }
}

bool Soldier::init() {
    init_soldier();

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
    _billboard->setPosition3D(Vec3(0.f, 100.f, 0.f));
    addChild(_billboard);

    scheduleUpdate();
    setCameraMask(World::CAMERA_I);
    return true;
}

#define AT_STATE(_state) if(atState(_state))

void Soldier::update(float dt) {
    auto world = World::getInstance();
    auto terrain = world->getTerrain();
    auto camera = world->getCamera();

    do {
    AT_STATE(SOLDIER_STATE_MOVE) {
        updateRotation();
        updatePosition(dt);
        break;
    }
    AT_STATE(SOLDIER_STATE_ACTION) {
        if (_cur_action->isDone()) {
            rmState(SOLDIER_STATE_ACTION);
        }
        break;
    }
    } while (false);

    if (this == s_followed) {
        camera->setPosition3D(getPosition3D() + getCameraOffset());
        camera->lookAt(getPosition3D());
    }
}

void Soldier::updateRotation() {
    Vec3 rota = getRotation3D();
    rota.y = _base_angle_z
            - CC_RADIANS_TO_DEGREES(_angle.getAngle());
    setRotation3D(rota);
}

void Soldier::updatePosition(float dt) {
    auto playerPos = getPosition3D();

    auto angle = _angle;
    /*//计算相对于摄像机的角度
    auto delta = g_world->getCamera()->getPosition3D() - getPosition3D();
    Vec2 delta2(delta.x, delta.z);
    angle.rotate(Vec2::ZERO, delta2.getAngle() - angle.getAngle() + 3.1415926535898 / 2.f); // */

    playerPos.x += _speed * angle.x * dt;
    playerPos.z -= _speed * angle.y * dt;

    auto playerModelMat = getParent()->getNodeToWorldTransform();
    playerModelMat.transformPoint(&playerPos);
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
    auto v = _camera_offset;
    v.normalize();
    _camera_offset += v * factor;
}

void Soldier::CameraRotate(Vec2 &v) {
    //Rotate X-Z coordinate
    if (fabs(v.x) > fabs(v.y)) {
        Vec2 v_xz(_camera_offset.x, _camera_offset.z);
        v_xz.rotate(Vec2::ZERO, v.x * 0.02);
        //
        _camera_offset.x = v_xz.x;
        _camera_offset.z = v_xz.y;
    }
    else {
        //Rotate Y-Z coordinate
        Vec2 v_yz(_camera_offset.y, _camera_offset.z);
        v_yz.rotate(Vec2::ZERO, v.y * 0.02);
        //
        _camera_offset.y = v_yz.x;
        _camera_offset.z = v_yz.y;
    }
}

void Soldier::show_blood_decline(float dec) {
    static Text *text = nullptr;
    if (!text) {
        text = Text::create("", "fonts/Marker Felt.ttf", 20);
        text->setCameraMask(World::CAMERA_I);
        _billboard->addChild(text);
    }
    char str[128];
    sprintf(str, "- %g", dec);
    text->setTextColor(Color4B(Color4F(255.f, 0.f, 0.f, 200.f)));
    text->setPosition(Vec2(0.f, 10.f));
    text->setScale(1);
    text->setString(str);
    text->setOpacity(255);

    auto move = MoveBy::create(1.5f, Vec2(0, 40.f));
    auto scale = ScaleBy::create(1.5f, 1.3f);
    //auto fade = FadeOut::create(1.5f);
    text->runAction(move);
    text->runAction(scale);
    //text->runAction(fade);
}

void Soldier::addThing(Node *node) {
    node->setCameraMask(World::CAMERA_I);
    addChild(node);
}