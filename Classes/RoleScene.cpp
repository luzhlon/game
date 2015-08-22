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
    auto layer = loadLayer("role_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setButtonClickCallback(layout, "button_start", CC_CALLBACK_1(RoleScene::onStartClick, this));
    setButtonClickCallback(layout, "button_back", CC_CALLBACK_1(RoleScene::onLeaveClick, this));

    m_editName = static_cast<TextField *>(Helper::seekWidgetByName(layout, "edit_name"));
	
    return true;
}

void RoleScene::onEnter() {
	Layer::onEnter();

    Client::getInstance()->start();
	//认证是否成功
    HANDLER(authentication) = Client::handler([](net_pkg *pkg) {
		if (pkg->arg1) { //成功，进入房间列表
            log("[Auth success]");
			Director::getInstance()->pushScene(RoomListScene::createScene());
		}
        else {
            log("[Auth failure]: %s", pkg->data);
        }
    });
}

void RoleScene::onStartClick(Ref *ref) {
    if(!Client::getInstance()->isConnected()) {
        log("Connect server failure.");
        return;
    }
	if (m_editName->getString().empty()) { //昵称不能为空
		log("nick name is empty");
		return;
	}
    Client::getInstance()->sendMsg(authentication, m_editName->getString().c_str());
}

void RoleScene::onLeaveClick(Ref *ref) {
    Director::getInstance()->popScene();
}
