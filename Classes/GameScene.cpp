﻿#include "World.h"
#include "Skill.h"
#include "Player.h"
#include "AppDelegate.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "WomanSoldier.h"
#include "ManSoldier.h"

using namespace cocostudio;

World *g_world;
Player *g_player;

extern Director *g_director;
extern FileUtils *g_file;
extern Soldier *g_self;

float GameScene::scale_cell = 0.1f;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

void GameScene::loadMapLayer() {
    _drawNode = DrawNode3D::create();
    g_world->addThing(_drawNode);
    _drawNode->setPosition3D(Vec3::ZERO);

    addChild(g_world);
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() ) {
        return false;
    }

    {
        g_player = Player::getInstance(WomanSoldier::create());
        g_world = World::getInstance();
        g_world->addThing(g_self);
    }

    _node_editor = CSLoader::createNode("game_scene.csb");

    loadMapLayer();
    loadUIlayer();

    return true;
}

extern void _LogSize(const char *desc, const Size& size);
extern void _LogVec3(const char *desc, Vec3& v3);

void GameScene::loadUIlayer() {
    m_layer_ui = loadLayer(_node_editor, 2);
    CC_ASSERT(m_layer_ui);
    m_layer_ui->removeFromParent();

    auto layout = getLayout(m_layer_ui);
    _text_debug = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_debug"));

    auto layout_layer = Helper::seekWidgetByName(layout, "layout_layer");
    layout_layer->addTouchEventListener(CC_CALLBACK_2(GameScene::onLayerTouched, this));
    //鼠标监听
    auto mouse_listener = EventListenerMouse::create();
    mouse_listener->onMouseScroll = CC_CALLBACK_1(GameScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);

	setTouchCallback(layout, "button_direction", CC_CALLBACK_2(GameScene::onDirectionTouched, this));
    setClickCallback(layout, "button_1", [this](Ref *ref) {
	});
    setClickCallback(layout, "button_2", [this](Ref *ref) {
		auto s = (WomanSoldier *)g_self;
		s->action_walk();
	});
    setClickCallback(layout, "button_3", [this](Ref *ref) {
		auto s = (WomanSoldier *)g_self;
        //draw cube
        Vec3 v[8];
        g_self->getAABB().getCorners(v);
        _drawNode->clear();
        _drawNode->drawCube(v, Color4F(0, 1, 0, 1));
	});
    setClickCallback(layout, "button_reset", [this](Ref *ref) {
		auto s = (WomanSoldier *)g_self;
        //g_self->show_blood_decline(10.8f);
        //g_player->draw_clear();
        //g_self->setPosition3D(Vec3::ZERO);
        //_LogVec3("[Soldier] Position: ", g_self->getPosition3D());
        //_LogVec3("[Soldier] Center: ", ((AABB&)(g_self->getAABB())).getCenter());
	});
    setClickCallback(layout, "button_camera", [this](Ref *ref) {
        //g_player->draw_circle(100.f);
        if (g_world->getCameraMask() == World::CAMERA_I) {
            g_world->setCameraMask(World::CAMERA_FIX);
        }
        else {
            g_world->setCameraMask(World::CAMERA_I);
            auto pos = g_self->getPosition3D();
            log("[Soldier pos] %g %g %g", pos.x, pos.y, pos.z);
        }
	});

    Skill::btn_skill_kick = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_kick"));
    Skill::btn_skill_boxing = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_boxing"));
    Skill::btn_skill_special = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_special"));

    setClickCallback(layout, "button_skill_kick", Skill::onSkillClicked);
    setClickCallback(layout, "button_skill_boxing", Skill::onSkillClicked);
    setClickCallback(layout, "button_skill_special", Skill::onSkillClicked);

    addChild(m_layer_ui);
}

void GameScene::onDirectionTouched(Ref *ref, Widget::TouchEventType type) {
    Button *btn = dynamic_cast<Button *>(ref);
    static Vec2 pos_began;
    static Vec2 pos_last;

    CC_ASSERT(btn);

    switch(type) {
    case Widget::TouchEventType::BEGAN:
    {
        pos_began = btn->getTouchBeganPosition();
        ui2gl(pos_began);
    }
    break;
    case Widget::TouchEventType::ENDED:
    {
        g_self->move_stop();
    }
    break;
    case Widget::TouchEventType::MOVED:
    {
        auto pos = btn->getTouchMovePosition();
        ui2gl(pos);
        auto dt = pos - pos_began;
    }
    break;
    }
}

void GameScene::ui2gl(Vec2& v) {
    auto size = g_director->getWinSize();
    v.y = size.height - v.y;
}

void GameScene::onLayerTouched(Ref *ref, Widget::TouchEventType type) {
    Widget *wig = dynamic_cast<Widget *>(ref);
    static Vec2 pos_began;
    static Vec2 pos_last;

    CC_ASSERT(wig);

    switch(type) {
    case Widget::TouchEventType::BEGAN:
    {
        pos_began = wig->getTouchBeganPosition();
        pos_last = pos_began;
        _LogSize("[Positon:] ", Size(pos_began.x, pos_began.y));
    }
    break;
    case Widget::TouchEventType::ENDED:
    {
        auto pos = wig->getTouchEndPosition();
        if (pos == pos_began) { //Not moved
            ui2gl(pos);
            Vec3 point(pos.x, pos.y, 0.f);
            if (g_world->conv2space(point)){
                g_world->showPoint(point);
                g_self->move(point);
            } else {
                log("[World] get Space coordinate failure.");
            }
        }
    }
    break;
    case Widget::TouchEventType::MOVED:
    {
        auto pos_cur = wig->getTouchMovePosition();
        auto dt = pos_cur - pos_last;

        auto cam = g_world->get_camera();
        if (World::CAMERA_I == g_world->getCameraMask()) {
            Vec2 angle = dt;
            ui2gl(dt);
            angle.x *= 0.1f;
            angle.y *= 0.1f;

            g_self->CameraRotate(angle);
        }
        else {
            auto cp = cam->getPosition3D();
            cp.x -= dt.x * 0.1;
            cp.z += dt.y * 0.1;
            cam->setPosition3D(cp);
        }

        pos_last = pos_cur;
    }
    break;
    }
}

void GameScene::onMouseScroll(Event* event) {
    auto e = (EventMouse *)event;
    if (World::CAMERA_I == g_world->getCameraMask()) {
        g_self->CameraZoom(e->getScrollY());
    }
    else {
        auto cam = g_world->get_camera();
        cam->setPositionY(cam->getPositionY() + e->getScrollY());
    }
}

void GameScene::update(float dt) {
    //auto player = g_self;
}