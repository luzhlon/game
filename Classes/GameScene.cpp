#include "AppDelegate.h"
#include "GameScene.h"
#include "WomanSoldier.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

float GameScene::scale_cell = 0.1f;

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

WomanSoldier *soldier;

void GameScene::loadMapLayer(Node *scene_node) {
    m_layer_map = scene_node->getChildByTag(1);
    CC_ASSERT(m_layer_map);
    m_layer_map->removeFromParent();

    m_map = TMXTiledMap::create("map/map.tmx");
	/*
    m_scroll = extension::ScrollView::create
            (Director::getInstance()->getWinSize() + Size(30, 20));
    m_scroll->setDirection(extension::ScrollView::Direction::BOTH);
    m_scroll->setMinScale(0.5);
    m_scroll->setMaxScale(1.2);
    m_scroll->setContentSize(m_map->getContentSize());
    m_scroll->addChild(m_map);
	// */
    m_layer_map->addChild(m_map);

    addChild(m_layer_map);
}
// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() ) {
        return false;
    }

    auto node = CSLoader::createNode("game_scene.csb");

    loadMapLayer(node);
    loadUIlayer(node);

    /*
    //鼠标监听
    auto mouse_listener = EventListenerMouse::create();
    mouse_listener->onMouseScroll = CC_CALLBACK_1(GameScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
    //触摸监听
    touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
	// */

    //*
    soldier = WomanSoldier::create();
    addSoldier(m_map, soldier);
    // */

    return true;
}

/*
const Vec2& GameScene::getLeftDownPos() {
    return pos_down;
}

const Vec2& GameScene::mouse2map(Vec2 &pos_mouse) {
    static Vec2 ret;
    Vec2 pos_map = m_scroll->getContentOffset();
    Vec2 delta = pos_mouse - pos_map;
    Size size_map = m_map->getContentSize(); //地图原大小
    Vec2 size = getMapSize(); //缩放后的地图大小
    Vec2 scale(delta.x / size.x, delta.y / size.y); //Delta 所占地图的比例
    ret.x = size_map.width * scale.x;
    ret.y = size_map.height * scale.y;
    return ret;
}

void GameScene::scaleMap(Vec2 focu, float n) {
    float old_scale = m_scroll->getZoomScale();
    float new_scale = old_scale + scale_cell * n;
    //auto size = getMapSize();
    m_scroll->setZoomScale(new_scale, true);

    //log("Current Scale: %1.2f, Map size: %.2f,%.2f , Mouse postion: %.2f,%.2f ", new_scale, m_map->getMapSize().width, m_map->getMapSize().height, e->getCursorX(), e->getCursorY());
}
// */

void GameScene::addSoldier(TMXTiledMap* map,Soldier *msoldier) { //将士兵添加到map中
    msoldier->setPosition(Vec2(g_frame_size.width / 1.5, g_frame_size.height / 1.5));
    map->addChild(msoldier);
    //m_map->reorderChild(msoldier, 3);
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
    Vec2 pos_cur = touch->getLocation();
    Vec2 delta = pos_cur - pos_down;

    log("Layer:%.2f %.2f Map:%.2f %.2f, Mouse:%.2f %.2f", getPosition().x, getPosition().y, m_map->getPosition().x, m_map->getPosition().y, pos_down.x, pos_down.y);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    {
        pos_down = touch->getLocation();
        mouse_down = true;
    }

    log("Layer:%.2f %.2f Map:%.2f %.2f, Mouse:%.2f %.2f", getPosition().x, getPosition().y, m_map->getPosition().x, m_map->getPosition().y, pos_down.x, pos_down.y);
    Node *cur_node = event->getCurrentTarget();
    //log("Current Node: %x", cur_node);
    auto pos = convertToWorldSpace(soldier->getPosition());
    //log("Soldier :%.2f %.2f, Mouse:%.2f %.2f", pos.x, pos.y, pos_down.x, pos_down.y);
    //log("Map size:%.2f %.2f View size:%.2f %.2f", getMapSize().x, getMapSize().y, view_size.x, view_size.y);
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    mouse_down = false;
    //soldier->MoveTo(touch->getLocation());
    Vec2 v = touch->getLocation();
    //soldier->setPosition(mouse2map(v));
}

void GameScene::onMouseScroll(Event* event) {
    EventMouse *e = (EventMouse *)event;
    float fscrol = e->getScrollY();
    //scaleMap(e->getLocation(), fscrol);
}

void GameScene::loadUIlayer(Node *scene_node) {
    m_layer_ui = loadLayer(scene_node, 2);
    CC_ASSERT(m_layer_ui);
    m_layer_ui->removeFromParent();

    auto layout = getLayout(m_layer_ui);
    auto btn = static_cast<Button *>
            (Helper::seekWidgetByName(layout, "button_direction"));
    btn->addTouchEventListener(CC_CALLBACK_2(GameScene::onDirectionTouched, this));

    setClickCallback(layout, "button_1", [this](Ref *ref) {
        log("[log Control]button_1 clicked.");
                     });
    setClickCallback(layout, "button_2", [this](Ref *ref) {
        log("[log Control]button_2 clicked.");
                     });

    addChild(m_layer_ui);
}

void GameScene::onDirectionTouched(Ref *ref, Widget::TouchEventType type) {
    Button *btn = dynamic_cast<Button *>(ref);
    static Vec2 pos_began;

    CC_ASSERT(btn);

    switch(type) {
    case Widget::TouchEventType::BEGAN:
    {
        pos_began = btn->getTouchBeganPosition();
        log("[log Direction]began pos: %.2f %.2f", pos_began.x, pos_began.y);
    }
        break;
    case Widget::TouchEventType::ENDED:
    {
        auto pos = btn->getTouchEndPos();
        log("[log Direction]end pos: %.2f %.2f", pos.x, pos.y);
    }
        break;
    case Widget::TouchEventType::MOVED:
    {
        auto pos = btn->getTouchMovePos();
        log("[log Direction]move pos: %.2f %.2f", pos.x, pos.y);
    }
        break;
    }
}
