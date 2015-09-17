#include "SettingScene.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

extern int   g_volume;
extern char  g_server_ip[32];

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
    m_textIP->setString(g_server_ip);
    m_slidVolum = static_cast<Slider *>(Helper::seekWidgetByName(layout, "slider_volume"));
    m_slidVolum->setPercent(g_volume);

    setClickCallback(layout, "button_ok", CC_CALLBACK_1(SettingScene::onOkClicked, this));
    setClickCallback(layout, "button_cancel", CC_CALLBACK_1(SettingScene::onCancelClicked, this));

    return true;
}

void SettingScene::saveSettings() {
    //Uncompleted
    strncpy(g_server_ip, m_textIP->getString().c_str(), 32);
    g_volume = m_slidVolum->getPercent();
}

void SettingScene::onOkClicked(Ref *ref) {
    saveSettings();
    Director::getInstance()->popScene();
}

void SettingScene::onCancelClicked(Ref *ref) {
    Director::getInstance()->popScene();
}
