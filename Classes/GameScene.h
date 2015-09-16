#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "SceneLayer.h"
#include "extensions/cocos-ext.h"
#include "DrawNode3D.h"

class World;

class GameScene : public SceneLayer {
    static float scale_cell;
public:
    static Scene* createScene();
    static void ui2gl(Vec2 &v);

    CREATE_FUNC(GameScene);

    void loadMapLayer(); //加载地图
    void loadUIlayer(); //

    void update(float dt);

    bool init() override;

private:
    extension::ScrollView *m_scroll;
    TMXTiledMap *m_map; //地图
    Node *m_layer_map;
    Node *_node_editor;
    Layer *m_layer_ui;
    Layer *m_sel_obj; //当前选中的对象

    DrawNode3D *_drawNode;

    Text *_text_debug;

    Vec2  pos_down; //触摸按下时的位置
    Vec2  pos_down_map; //触摸按下时地图的位置
    bool  mouse_down = false;

public:
    //
    void onDirectionTouched(Ref *, Widget::TouchEventType);
	void onLayerTouched(Ref *, Widget::TouchEventType);
    void onMouseScroll(Event* event);
};

#endif

