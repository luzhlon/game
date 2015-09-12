#include "World.h"
#include "Skill.h"
#include "Player.h"
#include "AppDelegate.h"
#include "GameScene.h"
#include "cocostudio/CocoStudio.h"

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
        g_file->addSearchPath("scene/dialog_scene");
        g_file->addSearchPath("scene/menu_scene");
        g_file->addSearchPath("scene/room_list_scene");
        g_file->addSearchPath("scene/role_scene");
        g_file->addSearchPath("scene/setting_scene");
        g_file->addSearchPath("scene/game_scene");

        g_file->getInstance()->addSearchPath("Particle3D/materials");
        g_file->getInstance()->addSearchPath("Particle3D/scripts");
        g_file->getInstance()->addSearchPath("Particle3D/textures");

        g_file->addSearchPath("character");


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
		auto s = (WomanSoldier *)g_self;
		s->action_throw();
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
    setClickCallback(layout, "button_4", [this](Ref *ref) {
		auto s = (WomanSoldier *)g_self;
        g_self->show_blood_decline(10.8f);
        //_LogVec3("[Soldier] Position: ", g_self->getPosition3D());
        //_LogVec3("[Soldier] Center: ", ((AABB&)(g_self->getAABB())).getCenter());
	});
    setClickCallback(layout, "button_5", [this](Ref *ref) {
		auto s = (WomanSoldier *)g_self;
        s->setSpeed(100.f);
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
        g_self->setSpeed(50);
        pos_began = btn->getTouchBeganPosition();
    }
    break;
    case Widget::TouchEventType::ENDED:
    {
        auto pos = btn->getTouchEndPosition();
        g_self->action_stop();
    }
    break;
    case Widget::TouchEventType::MOVED:
    {
        auto pos = btn->getTouchMovePosition();
        auto dt = pos - pos_began;
        g_self->action_move(dt);
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
        g_self->setSpeed(50);
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

        Vec2 angle = dt;
        ui2gl(dt);
        angle.x *= 0.1;
        angle.y *= 0.1;

        g_self->CameraRotate(angle);

        pos_last = pos_cur;
    }
    break;
    }
}

void GameScene::onMouseScroll(Event* event) {
    auto e = (EventMouse *)event;
    g_self->CameraZoom(e->getScrollY());
}

void GameScene::update(float dt) {
    //auto player = g_self;
}