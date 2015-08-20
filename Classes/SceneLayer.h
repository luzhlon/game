#ifndef SCENELAYER_H
#define SCENELAYER_H

#include "cocos2d.h"
#include "HorseSoldier.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SceneLayer : public Layer
{
public:
    SceneLayer();

protected:
    Node *loadLayer(const std::string& sceneFile, int layerTag = 1);
    inline Layout *getLayout(Node *layer, const std::string& name = "layout") {
        return layer ? static_cast<Layout *>(layer->getChildByName(name)) : nullptr;
    }
    bool setButtonClickCallback(Layout *layout,
                                const std::string& btnName,
                                const Widget::ccWidgetClickCallback& callback);
};

#endif // SCENELAYER_H
