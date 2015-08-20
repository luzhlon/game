#include "SettingScene.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

Scene *SettingScene::createScene() {
    auto scene = Scene::create();
    auto layer = SettingScene::create();

    scene->addChild(layer);

    return scene;
}

bool SettingScene::init() {
    auto layer = loadLayer("setting_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    m_textIP = static_cast<TextField *>
            (Helper::seekWidgetByName(layout, "text_ip"));
    m_textPort = static_cast<TextField *>
            (Helper::seekWidgetByName(layout, "text_port"));

    setButtonClickCallback(layout, "button_continue", CC_CALLBACK_1(SettingScene::onOkClicked, this));

    return true;
}

void SettingScene::onOkClicked(Ref *ref) {
    log("return from setting scene");
    Director::getInstance()->popScene();
}
