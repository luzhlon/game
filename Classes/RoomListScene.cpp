#include "cocostudio/CocoStudio.h"
#include "RoomListScene.h"

using namespace cocostudio;

Scene *RoomListScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomListScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomListScene::init() {
    auto layer = loadLayer("chooseroom_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setButtonClickCallback(layout, "button_createroom", CC_CALLBACK_1(RoomListScene::onCreateClick, this));
    setButtonClickCallback(layout, "button_join", CC_CALLBACK_1(RoomListScene::onEnterClick, this));
    setButtonClickCallback(layout, "button_back", CC_CALLBACK_1(RoomListScene::onLeaveClick, this));
    //*
    m_listRoom = static_cast<ListView *>(Helper::seekWidgetByName(layout, "listview_roomlist"));

    return true;
}

void RoomListScene::onEnter() {

}

void RoomListScene::onCreateClick(Ref *ref) {

}

void RoomListScene::onEnterClick(Ref *ref) {

}

void RoomListScene::onLeaveClick(Ref *ref) {
    log("leave room list");
    Director::getInstance()->popScene();
}
