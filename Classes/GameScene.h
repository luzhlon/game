#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "SceneLayer.h"
#include "extensions/cocos-ext.h"

class GameScene : public SceneLayer {
    static float scale_cell;
public:
    static Scene* createScene();
    CREATE_FUNC(GameScene);

    void loadMapLayer(Node *); //加载地图
    void loadUIlayer(Node *); //

    void updatePostion(float );

    void addSoldier(TMXTiledMap* map, Soldier *soldier);
    /*
    void scaleMap(Vec2 focu, float n = 1);  //缩放地图
    void prepMoveMap(); //准备移动地图
    void begainMoveMap(Vec2 delta); //开始移动地图
    bool borderDetected();
    const Vec2& mouse2map(Vec2 &mouse); //鼠标位置TO地图位置
    const Vec2& getLeftDownPos();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onMouseScroll(Event* event);
    // */

    bool init() override;

private:
    extension::ScrollView *m_scroll;
    TMXTiledMap *m_map; //地图
    Node *m_layer_map;
    Layer *m_layer_ui;
    Layer *m_sel_obj; //当前选中的对象

    Vec2  pos_down; //触摸按下时的位置
    Vec2  pos_down_map; //触摸按下时地图的位置
    bool  mouse_down = false;

public:
    //
    void onDirectionTouched(Ref *, Widget::TouchEventType);
};

#endif

