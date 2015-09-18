#include "MenuScene.h"
#include "SettingScene.h"
#include "RoleScene.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

Scene *MenuScene::createScene() {
    auto scene = Scene::create();
    auto layer = MenuScene::create();

    scene->addChild(layer);

    return scene;
}

void MenuScene::onSettingClick(Ref *ref) {
    log("enter setting");
    Director::getInstance()->pushScene(SettingScene::createScene());
}

void MenuScene::onMultiClick(Ref *ref) {
    log("enter role");
    Director::getInstance()->pushScene(RoleScene::createScene());
}

bool MenuScene::init() {
    auto layer = load_layer("menu_scene.csb");
    auto layout = get_layout(layer);

    CC_ASSERT(layout); //load layout failure

    setClickCallback(layout, "button_multi", CC_CALLBACK_1(MenuScene::onMultiClick, this));
    setClickCallback(layout, "button_setting", CC_CALLBACK_1(MenuScene::onSettingClick, this));
    setClickCallback(layout, "button_quit", CC_CALLBACK_1(MenuScene::onQuitClick, this));

    return true;
}

void MenuScene::onEnter() {
    Layer::onEnter();
    //加载配置
    SettingScene::load_settings();
}

void MenuScene::onQuitClick(Ref *ref) {
	Director::getInstance()->end();
}
