#include "SceneLayer.h"
#include "dialog.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

char SceneLayer::s_cur_scene;

SceneLayer::SceneLayer()
{
}

Node *SceneLayer::loadLayer(const std::string& sceneFile, int layerTag) {
    auto node = CSLoader::getInstance()->createNode(sceneFile);
    return node ?
                loadLayer(node, layerTag) : nullptr;
}

Layer *SceneLayer::loadLayer(Node *scene, int layerTag) {
    auto child = scene->getChildByTag(layerTag);
    if(child) {
        child->removeFromParent();
        addChild(child);
        return static_cast<Layer *>(child);
    } else
        return nullptr;
}

bool SceneLayer::setClickCallback(Layout *layout,
							const std::string& btnName,
							const Widget::ccWidgetClickCallback& callback) {
    auto widget = Helper::seekWidgetByName(layout, btnName);
    if(widget) {
        widget->addClickEventListener(callback);
        return true;
    } else
        return false;
}

bool SceneLayer::setTouchCallback(Layout *layout,
								const std::string& btnName,
								const Widget::ccWidgetTouchCallback& callback) {
    auto widget = Helper::seekWidgetByName(layout, btnName);
    if(widget) {
		widget->addTouchEventListener(callback);
        return true;
    } else
        return false;
}
