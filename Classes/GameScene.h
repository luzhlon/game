#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "SceneLayer.h"
#include "DrawNode3D.h"

class World;

class GameScene : public SceneLayer {
public:
    static Scene* createScene();
    static void ui2gl(Vec2 &v); // ui坐标 to opengl坐标

    CREATE_FUNC(GameScene);

    void load_world(); //加载3D世界
    void load_ui(Node *root); // 加载UI

    static ImageView *s_image_direction; // 小地图中的方向图标
    static LoadingBar *s_load_magic;
    static Button *s_btn_boxing;
    static Button *s_btn_kick;
    static Button *s_btn_special;
    static Button *s_btn_speed;
    static Text *s_text_score_red;
    static Text *s_text_score_blue;

    static void set_small_direction(float); // 设置小地图中的方向

    bool init() override;
private:
    /*
    Node *m_layer_map;
    Layer *m_layer_ui; // */

	Text *_output = nullptr;
	TextField *_input = nullptr;

    DrawNode3D *_drawNode;

public:
    //
	void onScrollTouched(Ref *, Widget::TouchEventType);
	void onLayerTouched(Ref *, Widget::TouchEventType);
    void onMouseScroll(Event* event);
};

#endif

