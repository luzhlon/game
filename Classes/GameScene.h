#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "HorseSoldier.h"
#include "../extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameScene : public Layer {
    static float scale_cell;
    static Vec2  view_size;
public:
    static Scene* createScene();
    CREATE_FUNC(GameScene);
    void addSoldier(TMXTiledMap* map, Soldier *soldier);
    void loadMap(); //加载地图
    void scaleMap(Vec2 focu, float n = 1);  //缩放地图
    void prepMoveMap(); //准备移动地图
    void begainMoveMap(Vec2 delta); //开始移动地图
    bool borderDetected();
    const Vec2& mouse2map(Vec2 &mouse); //鼠标位置TO地图位置
    const Vec2& getMapSize();
    const Vec2& getLeftDownPos();

    virtual bool init();

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onMouseScroll(cocos2d::Event* event);
private:
    extension::ScrollView *m_scroll;
    TMXTiledMap *m_map; //地图
    Node *m_sel_obj; //当前选中的对象

    Vec2  pos_down; //触摸按下时的位置
    Vec2  pos_down_map; //触摸按下时地图的位置
    bool  mouse_down = false;
};

#endif

