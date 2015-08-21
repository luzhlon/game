#include "RoomListScene.h"
#include "Dialog.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

Scene *RoomListScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomListScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomListScene::init() {
    auto layer = loadLayer("room_list_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setButtonClickCallback(layout, "button_create", CC_CALLBACK_1(RoomListScene::onCreateClick, this));
    setButtonClickCallback(layout, "button_enter", CC_CALLBACK_1(RoomListScene::onEnterClick, this));
    setButtonClickCallback(layout, "button_back", CC_CALLBACK_1(RoomListScene::onBackClick, this));
    //*
    m_listRoom = static_cast<ListView *>(Helper::seekWidgetByName(layout, "list_room"));

    return true;
}

void RoomListScene::onEnter() {
	Layer::onEnter();
	//获取房间列表
	//
}

void RoomListScene::onCreateClick(Ref *ref) {
	auto dlg = Dialog::getInstance();
	dlg->caption()->setString("创建房间");
	dlg->content_t()->setVisible(false);
    dlg->content_e()->setPlaceHolder("输入房间名称"); //
    dlg->setCallback(Dialog::Callback([this](Dialog *d, bool ok) {
        cocos2d::log("房间名：%s OK:%d",
                         d->content_e()->getString().c_str(), ok);
	}));
	dlg->Popup(this);
}

void RoomListScene::onEnterClick(Ref *ref) {

}

void RoomListScene::onBackClick(Ref *ref) {
    log("leave room list");
    Director::getInstance()->popScene();
}
