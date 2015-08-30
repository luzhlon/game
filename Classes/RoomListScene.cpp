#include "RoomListScene.h"
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
    log("[Log] item %s clicked", item.c_str());
}

bool RoomListScene::init() {
    auto layer = loadLayer("room_list_scene.csb");
    auto layout = getLayout(layer);

    CC_ASSERT(layout); //load layout failure

    setClickCallback(layout, "button_create", CC_CALLBACK_1(RoomListScene::onCreateClick, this));
    setClickCallback(layout, "button_enter", CC_CALLBACK_1(RoomListScene::onEnterClick, this));
    setClickCallback(layout, "button_back", CC_CALLBACK_1(RoomListScene::onBackClick, this));
    //*
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
    HANDLER(room_list) =
            Client::handler([this](net_pkg *pkg) {
            if(!IsCurScene(SCENE_ROOMLIST)) return;

            m_listRoom->removeAllItems();
            char *p = pkg->data;
            log("[Room num:] %d", pkg->arg1);
            for(int i = 0; i < pkg->arg1; i++) {
                log("[Room:] %s", p);
                addRoomItem(p);
                p += strlen(p) + 1;
            }
    });
    clt->sendMsg((msg_msg)MESSAGE::room_list);
}

void RoomListScene::onCreateClick(Ref *ref) {
	auto dlg = Dialog::getInstance();
    dlg->setCallback(Dialog::Callback([this](Dialog *d, bool ok) {
        auto name = d->content_e()->getString();
        cocos2d::log("房间名：%s OK:%d",
                         name.c_str(), ok);
        if(name.empty()) {
            cocos2d::log("房间名is empty.");
            return;
        }
        HANDLER(create_room) = Client::handler(
                         [](net_pkg *pkg) {
                        //uncomplete
                             if(pkg->arg1) {
                                 log("create room success");
                             } else {
                                 log("create room success");
                             }
                         });
        Client::getInstance()->sendMsg(MESSAGE::create_room, name.c_str());
		updateRoomList();
    }));
    dlg->Popup_e(this, "创建房间", "Room name");
}

void RoomListScene::onEnterClick(Ref *ref) {
    //uncomplete
	Director::getInstance()->pushScene(GameScene::createScene());
}

void RoomListScene::onUpdateClick(Ref *) {
	updateRoomList();
}

void RoomListScene::onBackClick(Ref *ref) {
    log("leave room list");
    Director::getInstance()->popScene();
}
