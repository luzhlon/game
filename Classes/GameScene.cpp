﻿#include "World.h"
#include "Skill.h"
#include "Client.h"
#include "Dialog.h"
#include "Player.h"
#include "AppDelegate.h"
#include "GameScene.h"
#include "NetRoom.h"
#include "cocostudio/CocoStudio.h"
#include "Soldier.h"
#include "../Server/message.h"

using namespace cocostudio;

World *g_world;
Player *g_player;
Client *g_client;

extern Director *g_director;
extern FileUtils *g_file;
extern Soldier *g_self;

Soldier *g_soldiers[MAX_ROOM_MEMBERS];

GameScene *GameScene::Instance = nullptr;
Layout *GameScene::s_layout_ui = nullptr;
ImageView *GameScene::s_image_direction = nullptr; // 小地图中的方向图标
ImageView *GameScene::s_image_progress = nullptr; // 读条
LoadingBar *GameScene::s_load_progress = nullptr;
LoadingBar *GameScene::s_load_magic = nullptr;
Button *GameScene::s_btn_boxing = nullptr;
Button *GameScene::s_btn_kick = nullptr;
Button *GameScene::s_btn_special = nullptr;
Button *GameScene::s_btn_speed = nullptr;
Text *GameScene::s_text_score_red = nullptr;
Text *GameScene::s_text_score_blue = nullptr;
Text *GameScene::s_text_grass = nullptr;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

void GameScene::load_world() {
    _drawNode = DrawNode3D::create();
    g_world->add_thing(_drawNode);
    _drawNode->setPosition3D(Vec3::ZERO);

    addChild(g_world);
}

void GameScene::set_score_red(int score) {
    char buf[16];
    sprintf(buf, "%u", score);
    s_text_score_red->setString(buf);
}

void GameScene::set_score_blue(int score) {
    char buf[16];
    sprintf(buf, "%u", score);
    s_text_score_blue->setString(buf);
}

void GameScene::set_score(int score) {
    char buf[16];
    sprintf(buf, "%u", score);
    s_text_grass->setString(buf);
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    Instance = this;
    if ( !Layer::init() ) {
        return false;
    }
    g_world = World::getInstance();
    g_client = Client::getInstance();

    auto node = CSLoader::createNode("game_scene.csb");

    load_world();
    load_ui(node);

    NetRoom::init();

    /* Have BUG
    Client::onDisconnect = [this]() {
        Dialog::getInstance()->setCallback(Dialog::Callback([](Dialog *dlg, bool ok) {
            Director::getInstance()->popScene();
        }));
        Dialog::getInstance()->Popup_t(this, "ERROR", "与服务器失去连接");
    }; // */

    return true;
}

extern void _LogSize(const char *desc, const Size& size);
extern void _LogVec3(const char *desc, Vec3& v3);

void GameScene::load_ui(Node *root) {
    auto layer_ui = load_layer(root, 2);
    //CC_ASSERT(m_layer_ui);
    layer_ui->removeFromParent();
    addChild(layer_ui);

    auto layout = get_layout(layer_ui);
    s_layout_ui = layout;

	setTouchCallback(layout, "layout_layer", CC_CALLBACK_2(GameScene::onLayerTouched, this));
	setTouchCallback(layout, "layout_scroll", CC_CALLBACK_2(GameScene::onScrollTouched, this));

	_output = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_output"));
	_input = static_cast<TextField *>(Helper::seekWidgetByName(layout, "edit_input"));
    //CC_ASSERT(_output);
    //CC_ASSERT(_input);
    s_image_direction = static_cast<ImageView *>(Helper::seekWidgetByName(layout, "image_direction"));
    //CC_ASSERT(s_image_direction);
    s_btn_kick = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_kick"));
    s_btn_kick->addClickEventListener(Player::onSkillClicked);
    s_btn_boxing = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_boxing"));
    s_btn_boxing->addClickEventListener(Player::onSkillClicked);
    s_btn_special = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_special"));
    s_btn_special->addClickEventListener(Player::onSkillClicked);
    s_btn_speed = static_cast<Button *>(Helper::seekWidgetByName(layout,  "button_skill_speed"));
    s_btn_speed->addClickEventListener(Player::onSkillClicked);

    s_load_magic = static_cast<LoadingBar *>(Helper::seekWidgetByName(layout,  "loadbar_magic"));
    CC_ASSERT(s_load_magic);
    s_text_score_red = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_score_red"));
    CC_ASSERT(s_text_score_red);
    s_text_score_blue = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_score_blue"));
    CC_ASSERT(s_text_score_blue);
    s_text_grass = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_grass_num"));
    CC_ASSERT(s_text_grass);
    s_image_progress = static_cast<ImageView *>(Helper::seekWidgetByName(layout, "image_progress"));
    CC_ASSERT(s_image_progress);
    s_load_progress = static_cast<LoadingBar *>(Helper::seekWidgetByName(layout, "loadbar_progress"));
    CC_ASSERT(s_load_progress);

    //鼠标监听
    auto mouse_listener = EventListenerMouse::create();
    mouse_listener->onMouseScroll = CC_CALLBACK_1(GameScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);

    setClickCallback(layout, "button_pick_up", CC_CALLBACK_1(GameScene::onPickUpClicked, this));
	//setTouchCallback(layout, "button_direction", CC_CALLBACK_2(GameScene::onDirectionTouched, this));
    setClickCallback(layout, "button_1", [this](Ref *ref) {
	});
    setClickCallback(layout, "button_2", [this](Ref *ref) {
        g_self->show_blood_decline(35.f);
		//s->action_walk();
	});
    setClickCallback(layout, "button_3", [this](Ref *ref) {
        //draw cube
        Vec3 v[8];
        g_self->getAABB().getCorners(v);
        _drawNode->clear();
        _drawNode->drawCube(v, Color4F(0, 1, 0, 1));
	});
    setClickCallback(layout, "button_reset", [this](Ref *ref) {
		World::s_camera_offset = Vec3(0, 120, 90);
	});
    setClickCallback(layout, "button_camera", [this](Ref *ref) {
        //g_player->draw_circle(100.f);
        if (g_world->getCameraMask() == World::CAMERA_FREE) {
            g_world->setCameraMask(World::CAMERA_FIX);
        } else {
            g_world->setCameraMask(World::CAMERA_FREE);
            auto pos = g_self->getPosition3D();
            log("[Soldier pos] %g %g %g", pos.x, pos.y, pos.z);
        }
	});

}

void GameScene::ui2gl(Vec2& v) {
    auto size = g_director->getWinSize();
    v.y = size.height - v.y;
}

void GameScene::onLayerTouched(Ref *ref, Widget::TouchEventType type) {
    Widget *wig = dynamic_cast<Widget *>(ref);
    static Vec2 pos_began;
    static Vec2 pos_last;

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
                Vec2 v2(point.x, point.z);
                NetRoom::action_move(v2);
                point.y += 1.f;
                g_world->show_click(point);
            } else {
                log("[World] get Space coordinate failure.");
            }
        } else {
            // 自由视角下设置人物朝向
            if (g_world->getCameraMask() == World::CAMERA_FREE &&
                g_self->state() == Soldier::SOLDIER_STATE_IDLE) {

                Vec2 delta(World::s_camera_offset.x, World::s_camera_offset.z);
                auto angle = CC_RADIANS_TO_DEGREES(delta.getAngle());
                angle += 180.f;
                NetRoom::set_angle(angle);
            }
        }
    }
    break;
    case Widget::TouchEventType::MOVED:
    {
        auto pos_cur = wig->getTouchMovePosition();
        auto dt = pos_cur - pos_last;

        g_world->camera_move(dt);

        pos_last = pos_cur;
    }
    break;
    }
}

void GameScene::onScrollTouched(Ref *ref, Widget::TouchEventType type) {
    Widget *wig = dynamic_cast<Widget *>(ref);
    static Vec2 pos_began;
    static Vec2 pos_last;

    switch(type) {
    case Widget::TouchEventType::BEGAN:
    {
        pos_began = wig->getTouchBeganPosition();
        pos_last = pos_began;
    }
    break;
    case Widget::TouchEventType::ENDED:
    {
        auto pos = wig->getTouchEndPosition();
    }
    break;
    case Widget::TouchEventType::MOVED:
    {
        auto pos_cur = wig->getTouchMovePosition();
        auto dt = pos_cur - pos_last;

		g_world->camera_zoom(dt.y > 0.f ? 1.f : -1.f);
		char buf[64];
		auto v3 = World::s_camera_offset;
		sprintf(buf, "%g %g %g", v3.x, v3.y, v3.z);
		_output->setString(buf);

        pos_last = pos_cur;
    }
    break;
    }
}

void GameScene::onMouseScroll(Event* event) {
    auto e = (EventMouse *)event;
    g_world->camera_zoom(e->getScrollY());
}

void GameScene::set_small_direction(float angle) {
    s_image_direction->runAction(RotateTo::create(0.1f, angle));
}

void GameScene::onPickUpClicked(Ref *ref) {
    g_player->on_pick_goods();
}

void GameScene::begain_progress(float seconds,
    std::function<void(float dt, bool end)> cb) {
    //s_image_progress->setVisible(true);
    set_progress(0.f);
    show_progress(true);
    _prog_second = fabs(seconds);
    _cur_second = 0.f;
    _prog_cb = cb;
    schedule(schedule_selector(GameScene::update_progress), 0.2f);
}

void GameScene::break_progress() {
    show_progress(false);
    unschedule(schedule_selector(GameScene::update_progress));
}

void GameScene::update_progress(float dt) {
    _cur_second += dt;
    if (_cur_second > _prog_second) { // 读条完毕
        break_progress();
        if (_prog_cb) _prog_cb(dt, true);
        _prog_cb = nullptr;
    } else{
        set_progress(_cur_second / _prog_second * 100.f);
        if (_prog_cb) _prog_cb(dt, false);
    }
}
