#include "MainScene.h"
#include "HorseSoldier.h"

float MainScene::scale_cell = 0.1f;
Vec2 MainScene::view_size = Vec2(0.0, 0.0);

Scene* MainScene::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

EventListenerTouchOneByOne *touch_listener;
HorseSoldier *soldier;

void MainScene::loadMap() {
    m_scroll = ScrollView::create(Director::getInstance()->getOpenGLView()->getVisibleSize() + Size(30, 20));
    //m_map = TMXTiledMap::create("iceworld.tmx");
    m_map = TMXTiledMap::create("map.tmx");
    m_scroll->setDirection(ScrollView::Direction::BOTH);
    m_scroll->setMinScale(0.5);
    m_scroll->setMaxScale(1.2);
    m_scroll->setContentSize(m_map->getContentSize());
    m_scroll->addChild(m_map);
    addChild(m_scroll);
}
// on "init" you need to initialize your instance
bool MainScene::init()
{
    if ( !Layer::init() ) {
        return false;
    }
	//View大小
	auto glview = Director::getInstance()->getOpenGLView();
	auto size = glview->getVisibleSize();
	view_size.x = size.width;
	view_size.y = size.height;
	
	loadMap();

	//鼠标监听
	auto mouse_listener = EventListenerMouse::create();
	mouse_listener->onMouseScroll = CC_CALLBACK_1(MainScene::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
	//触摸监听
	touch_listener = EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);

	soldier = HorseSoldier::create();
	addSoldier(m_map, soldier);

    return true;
}

const Vec2& MainScene::getLeftDownPos() {
	return pos_down;
}

const Vec2& MainScene::getMapSize() {
	static Vec2 map_size;
	auto size = m_map->getContentSize();
	auto scale = m_scroll->getZoomScale();
  map_size.x = size.width * scale;
	map_size.y = size.height * scale;
	return map_size;
}

const Vec2& MainScene::mouse2map(Vec2 &pos_mouse) {
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

void MainScene::scaleMap(Vec2 focu, float n) {
	float old_scale = m_scroll->getZoomScale();
	float new_scale = old_scale + scale_cell * n;
	auto size = getMapSize();
	m_scroll->setZoomScale(new_scale, true);

	//log("Current Scale: %1.2f, Map size: %.2f,%.2f , Mouse postion: %.2f,%.2f ", new_scale, m_map->getMapSize().width, m_map->getMapSize().height, e->getCursorX(), e->getCursorY());
}

void MainScene::prepMoveMap() {
	pos_down_map = m_map->getPosition();
}

void MainScene::begainMoveMap(Vec2 delta) {
	Vec2 pos = pos_down_map + delta;
	//超出左下角的处理
	pos.x = pos.x > 0.0 ? 0.0 : pos.x;
	pos.y = pos.y > 0.0 ? 0.0 : pos.y;
	//超出右上角的处理
	Vec2 ru_pos = pos + getMapSize(); 
	pos.x += ru_pos.x < view_size.x ? view_size.x - ru_pos.x : 0.0;
	pos.y += ru_pos.y < view_size.y ? view_size.y - ru_pos.y : 0.0;
	//
	m_map->setPosition(pos);
}

void MainScene::addSoldier(TMXTiledMap* map,Soldier *msoldier){ //将士兵添加到map中
	msoldier->setPosition(Vec2(view_size.x / 2, view_size.y / 2));
	map->addChild(msoldier);
    m_map->reorderChild(msoldier, 3);
}

void MainScene::onTouchMoved(Touch* touch, Event* event){
	Vec2 pos_cur = touch->getLocation();
	Vec2 delta = pos_cur - pos_down;
}

bool MainScene::onTouchBegan(Touch* touch, Event* event){
	{
		pos_down = touch->getLocation();
		mouse_down = true;
	}

	Node *cur_node = event->getCurrentTarget();
	//log("Current Node: %x", cur_node);
	//log("Layer:%.2f %.2f Map:%.2f %.2f, Mouse:%.2f %.2f", getPosition().x, getPosition().y, m_map->getPosition().x, m_map->getPosition().y, pos_down.x, pos_down.y);
	auto pos = convertToWorldSpace(soldier->getPosition());
	//log("Soldier :%.2f %.2f, Mouse:%.2f %.2f", pos.x, pos.y, pos_down.x, pos_down.y);
	log("Map size:%.2f %.2f View size:%.2f %.2f", getMapSize().x, getMapSize().y, view_size.x, view_size.y);
	return true;
}

void MainScene::onTouchEnded(Touch* touch, Event* event){
	mouse_down = false;
	//soldier->MoveTo(touch->getLocation());
    Vec2 v = touch->getLocation();
    soldier->setPosition(mouse2map(v));
}

void MainScene::onMouseScroll(Event* event){
	EventMouse *e = (EventMouse *)event;
	float fscrol = e->getScrollY();
	scaleMap(e->getLocation(), fscrol);
}
