#include "RoomScene.h"

Scene *RoomScene::createScene() {
    auto scene = Scene::create();
    auto layer = RoomScene::create();

    scene->addChild(layer);

    return scene;
}

bool RoomScene::init() {
    return true;
}
