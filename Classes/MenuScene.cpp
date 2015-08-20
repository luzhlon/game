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
    auto layer = loadLayer("menu_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setButtonClickCallback(layout, "button_multi", CC_CALLBACK_1(MenuScene::onMultiClick, this));
    setButtonClickCallback(layout, "button_setting", CC_CALLBACK_1(MenuScene::onSettingClick, this));

    return true;
}
