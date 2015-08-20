#include "RoleScene.h"
#include "RoomListScene.h"
#include "Client.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
using namespace MESSAGE;

Scene *RoleScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoleScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoleScene::init() {
    auto layer = loadLayer("select_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setButtonClickCallback(layout, "button_start", CC_CALLBACK_1(RoleScene::onStartClick, this));
    setButtonClickCallback(layout, "button_back", CC_CALLBACK_1(RoleScene::onLeaveClick, this));

    m_editName = static_cast<TextField *>(Helper::seekWidgetByName(layout, "text_spritename"));

    return true;
}

void RoleScene::onEnter() {
    Client::getInstance()->start();
    HANDLER(authentication) = Client::handler([](net_pkg *pkg) {
        if(pkg->arg1)
            log("authentication success");
        else
            log("authentication failure");
        Director::getInstance()->pushScene(RoomListScene::createScene());
    });
}

void RoleScene::onStartClick(Ref *ref) {
    if(!Client::getInstance()->isConnected()) {
        log("Connect server failure.");
        return;
    }
    Client::getInstance()->sendMsg(authentication, m_editName->getString().c_str());
}

void RoleScene::onLeaveClick(Ref *ref) {
    Director::getInstance()->popScene();
}
