#include "RoomScene.h"

string RoomScene::s_room_name;

Scene *RoomScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomScene::init() {
    auto layer = loadLayer("room_list_scene.csb");
    auto layout = getLayout(layer);
        
    CC_ASSERT(layout); //load layout failure

    setClickCallback(layout, "check_ready", CC_CALLBACK_1(RoomScene::onReadyClick, this));
    setClickCallback(layout, "check_team", CC_CALLBACK_1(RoomScene::onTeamClick, this));

    CC_ASSERT(!s_room_name.empty());
    auto text_name = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_name"));
    text_name->setString(s_room_name);

    return true;
}

void RoomScene::onEnter() {
    Layer::onEnter();
    CurScene(SCENE_ROOM);
}

void RoomScene::onReadyClick(Ref *) {

}
void RoomScene::onTeamClick(Ref *) {

}
