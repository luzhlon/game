#ifndef SCENELAYER_H
#define SCENELAYER_H

#include "cocos2d.h"
#include "HorseSoldier.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SceneLayer : public Layer
{
    static char s_cur_scene;
public:
    SceneLayer();
    inline static void CurScene(char scene) {
        s_cur_scene = scene;
    }
    inline static char CurScene() {
        return s_cur_scene;
    }
    inline static bool IsCurScene(char scene) {
        return s_cur_scene == scene;
    }

    enum {
        SCENE_MENU,
        SCENE_SETTING,
        SCENE_ROLE,
        SCENE_ROOMLIST,
        SCENE_ROOM,
        SCENE_GAME
    };

protected:
    Node *loadLayer(const std::string& sceneFile, int layerTag = 1);
    Layer *loadLayer(Node *scene, int layerTag = 1);
    inline Layout *getLayout(Node *layer, const std::string& name = "layout") {
        return layer ? static_cast<Layout *>(layer->getChildByName(name)) : nullptr;
    }
    bool setClickCallback(Layout *layout,
                          const std::string& btnName,
                          const Widget::ccWidgetClickCallback& callback);
};

#endif // SCENELAYER_H
