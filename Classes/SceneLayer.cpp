#include "SceneLayer.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

SceneLayer::SceneLayer()
{
}

Node *SceneLayer::loadLayer(const std::string& sceneFile, int layerTag) {
    auto node = CSLoader::getInstance()->createNode(sceneFile);
    auto child = node->getChildByTag(layerTag);
    if(node) {
        child->removeFromParent();
        addChild(child);
        return child;
    } else
        return nullptr;
}

bool SceneLayer::setButtonClickCallback(Layout *layout,
                                        const std::string& btnName,
                                        const Widget::ccWidgetClickCallback& callback) {
    auto btn = static_cast<Button *>(Helper::seekWidgetByName(layout, btnName));
    if(btn) {
        btn->addClickEventListener(callback);
        return true;
    } else
        return false;
}
