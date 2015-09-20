#include "World.h"
#include "Player.h"
#include "AppDelegate.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "Soldier.h"
#include "../Server/message.h"

using namespace cocostudio;

World *g_world;
Player *g_player;

extern Director *g_director;
extern FileUtils *g_file;
extern Soldier *g_self;

enum {
OP_STATE_SECLET = 0,
OP_STATE_SOLDIER,

OP_STATE_NUM_MAX 
};

int g_op_state = OP_STATE_SOLDIER;

ImageView *GameScene::s_image_direction = nullptr; // 小地图中的方向图标

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


// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    g_world = World::getInstance();

    _node_editor = CSLoader::createNode("game_scene.csb");

    load_world();
    load_ui();

    auto sol = WomanSoldier::create();
    sol->begin_fight();
    g_world->add_thing(sol);
    Player::getInstance(sol);

    return true;
}

extern void _LogSize(const char *desc, const Size& size);
extern void _LogVec3(const char *desc, Vec3& v3);

void GameScene::load_ui() {
    m_layer_ui = load_layer(_node_editor, 2);
    CC_ASSERT(m_layer_ui);
    m_layer_ui->removeFromParent();

    auto layout = get_layout(m_layer_ui);

	setTouchCallback(layout, "layout_layer", CC_CALLBACK_2(GameScene::onLayerTouched, this));
	setTouchCallback(layout, "layout_scroll", CC_CALLBACK_2(GameScene::onScrollTouched, this));

	_output = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_output"));
	_input = static_cast<TextField *>(Helper::seekWidgetByName(layout, "edit_input"));
    CC_ASSERT(_output);
    CC_ASSERT(_input);

    s_image_direction = static_cast<ImageView *>(Helper::seekWidgetByName(layout, "image_direction"));
    CC_ASSERT(s_image_direction);
    //鼠标监听
    auto mouse_listener = EventListenerMouse::create();
    mouse_listener->onMouseScroll = CC_CALLBACK_1(GameScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);

	//setTouchCallback(layout, "button_direction", CC_CALLBACK_2(GameScene::onDirectionTouched, this));
    setClickCallback(layout, "button_1", [this](Ref *ref) {
        g_op_state++;
        if (g_op_state == OP_STATE_NUM_MAX) g_op_state = 0;
	});
    setClickCallback(layout, "button_2", [this](Ref *ref) {
		auto s = (WomanSoldier *)g_self;
        g_self->show_blood_decline(35.f);
		//s->action_walk();
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
		World::s_camera_offset = Vec3(0, 120, 90);
	});
    setClickCallback(layout, "button_camera", [this](Ref *ref) {
        //g_player->draw_circle(100.f);
        if (g_world->getCameraMask() == World::CAMERA_FREE) {
            g_world->setCameraMask(World::CAMERA_FIX);
        }
        else {
            g_world->setCameraMask(World::CAMERA_FREE);
            auto pos = g_self->getPosition3D();
            log("[Soldier pos] %g %g %g", pos.x, pos.y, pos.z);
        }
	});

    addChild(m_layer_ui);
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
            if (!g_world->conv2space(point))
            {
                log("[World] get Space coordinate failure.");
                break;
            }

            switch (g_op_state) {
            case OP_STATE_SECLET:
            {
                auto ps = point;
                auto pe = point;
                ps.y = 100.f;
                pe.y = -100.f;
                g_world->draw_node()->drawLine(ps, pe, Color4F(0,0,0,0));
                break;
            }
            case OP_STATE_SOLDIER:
                g_self->move(point);
                point.y += 1.f;
                g_world->show_click(point);
                char buf[64];
                sprintf(buf, "%g %g", point.x, point.z);
                _output->setString(buf);
                break;
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

		g_world->camera_zoom(dt.y > 0.f ? 1.5f : -1.5f);
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
    g_world->camera_zoom(e->getScrollY() * 3);
}

void GameScene::set_small_direction(float angle) {
    s_image_direction->runAction(RotateTo::create(0.3f, angle));
}
