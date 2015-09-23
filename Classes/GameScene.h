#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "SceneLayer.h"
#include "DrawNode3D.h"
#include <functional>

class World;

class GameScene : public SceneLayer {
public:
    static Scene* createScene();
    static void ui2gl(Vec2 &v); // ui坐标 to opengl坐标

    CREATE_FUNC(GameScene);

    void load_world(); //加载3D世界
    void load_ui(Node *root); // 加载UI

    static GameScene *Instance;
    static Layout *s_layout_ui;
    static ImageView *s_image_direction; // 小地图中的方向图标
    static ImageView *s_image_progress; // 读条
    static LoadingBar *s_load_progress;
    static LoadingBar *s_load_magic;
    static Button *s_btn_boxing;
    static Button *s_btn_kick;
    static Button *s_btn_special;
    static Button *s_btn_speed;
    static Text *s_text_score_red;
    static Text *s_text_score_blue;
    static Text *s_text_grass;

    static void set_small_direction(float); // 设置小地图中的方向
    static void set_score_red(int);
    static void set_score_blue(int);
    static void set_score(int);
    inline static void show_progress(bool s) {
        s_image_progress->setVisible(s);
    }
    inline static void set_progress(float percent) {
        s_load_progress->setPercent(percent);
    }

    float _prog_second;
    float _cur_second;
    std::function<void(float , bool end)> _prog_cb;
    void begain_progress(float seconds,
        std::function<void (float, bool end)> cb = nullptr);
    void break_progress();
    void update_progress(float dt);

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
    void onPickUpClicked(Ref *);
};

#endif

