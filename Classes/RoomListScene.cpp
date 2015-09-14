﻿#include "RoomListScene.h"
#include "RoomScene.h"
#include "GameScene.h"
#include "Dialog.h"
#include "Client.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

Scene *RoomListScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomListScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomListScene::addRoomItem(const string& item_name) {
    auto node = CSLoader::createNode("list_item.csb");
    auto layout = static_cast<Layout *>(node->getChildByName("layout"));
    CC_ASSERT(layout);
    if(!layout) return false;
    layout->removeFromParent();
    auto btn = static_cast<Button *>
            (Helper::seekWidgetByName(layout, "button_item"));
    btn->setTitleText(item_name);
    btn->addClickEventListener(CC_CALLBACK_1(RoomListScene::onItemClick, this));

    auto size = m_listRoom->getSize();
    layout->setSize(Size(size.width * 0.8f, size.height * 0.15f));
    m_listRoom->pushBackCustomItem(layout);

    return true;
}

void RoomListScene::onItemClick(Ref *ref) {
    Button *btn = dynamic_cast<Button *>(ref);
    CC_ASSERT(btn);
    string item = btn->getTitleText();
    HANDLER(join_room) = Client::handler([this, &item](net_pkg *pkg) {
        if (pkg->arg1) {
            RoomScene::s_room_name = item;
            Director::getInstance()->pushScene(RoomScene::createScene());
        } else {
            Dialog::getInstance()->Popup_t(this, "错误", "加入房间失败：" + string(pkg->data));
        }
    });
    Client::getInstance()->sendMsg(MESSAGE::join_room, item.c_str());
    log("[Log] item %s clicked", item.c_str());
}

bool RoomListScene::init() {
    auto layer = loadLayer("room_list_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setClickCallback(layout, "button_create", CC_CALLBACK_1(RoomListScene::onCreateClick, this));
    setClickCallback(layout, "button_update", CC_CALLBACK_1(RoomListScene::onUpdateClick, this));
    setClickCallback(layout, "button_back", CC_CALLBACK_1(RoomListScene::onBackClick, this));

    m_listRoom = static_cast<ListView *>(Helper::seekWidgetByName(layout, "list_room"));

    return true;
}

void RoomListScene::onEnter() {
	Layer::onEnter();
    CurScene(SCENE_ROOMLIST);
    //set timer
	/*
    schedule([this](float lt) {
        updateRoomList();
    }, 1.0f, "timer"); // */
    //房间列表
    updateRoomList();
}

void RoomListScene::onExit() {
    Layer::onExit();
    //unset timer
    //unschedule("timer");
}

void RoomListScene::updateRoomList() {
    auto clt = Client::getInstance();
    m_listRoom->removeAllItems();
    HANDLER(room_list) =
            Client::handler([this](net_pkg *pkg) {
        if (pkg->arg1) {
            //pkg->data[pkg->arg2] = '\0';
            addRoomItem(pkg->data);
        } else { //房间枚举结束
            HANDLER(room_list) = nullptr;
        }
    });
    clt->sendMsg((msg_msg)MESSAGE::room_list);
}

void RoomListScene::onCreateClick(Ref *ref) {
	auto dlg = Dialog::getInstance();
    dlg->setCallback(Dialog::Callback([this](Dialog *d, bool ok) {
        if (!ok) return; //点击了取消按钮
        auto name = d->content_e()->getString();
        cocos2d::log("房间名：%s OK:%d",
                         name.c_str(), ok);
        if(name.empty()) {
            cocos2d::log("房间名is empty.");
            return;
        }
        HANDLER(create_room) = Client::handler(
                         [this](net_pkg *pkg) {
                             if(pkg->arg1) {
                                 log("create room success");
                                 updateRoomList();
                             } else {
                                 Dialog::getInstance()->Popup_t(this, "Create room failure", pkg->data);
                             }
                         });
        Client::getInstance()->sendMsg(MESSAGE::create_room, name.c_str());
    }));
    dlg->Popup_e(this, "创建房间", "Room name");
}

void RoomListScene::onUpdateClick(Ref *) {
	updateRoomList();
}

void RoomListScene::onBackClick(Ref *ref) {
    log("leave room list");
    Director::getInstance()->popScene();
}
