﻿#include "RoleScene.h"
#include "RoomListScene.h"
#include "Client.h"
#include "Dialog.h"
#include "Soldier.h"
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

    setClickCallback(layout, "button_start", CC_CALLBACK_1(RoleScene::onStartClick, this));
    setClickCallback(layout, "button_back", CC_CALLBACK_1(RoleScene::onLeaveClick, this));

    m_editName = static_cast<TextField *>
            (Helper::seekWidgetByName(layout, "edit_name"));
    m_pageSprite = static_cast<PageView*>
            (Helper::seekWidgetByName(layout, "page_sprite"));

    load_pages();
	
    return true;
}

bool RoleScene::load_pages() {
    auto _layout = static_cast<Layout *>
        (Helper::seekWidgetByName(m_pageSprite, "layout_sprite"));
    _layout->retain();

    auto size = m_pageSprite->getSize();
    m_pageSprite->removeAllPages();

    for(int i = 0; i < Soldier::Type::TYPE_NUMBER; i++) {
        //Layout *layout;
        Layout* layout = (Layout *)_layout->clone();
        m_pageSprite->addPage(layout);
        layout->setSizePercent(Vec2(1.f, 1.f));
        //layout->setPositionPercent(Vec2(1.f, 1.f));

        auto s = Soldier::create(i);
        s->setScale(1.f);
        layout->addChild(s);
        s->setPosition(Vec2(size.width / 2.f, size.height / 3.f));

        auto role_name = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_role_name"));
        role_name->setPositionPercent(Vec2(0.5f, 0.05f));
        role_name->setString(s->name());
    }
    return true;
}

void RoleScene::onEnter() {
	Layer::onEnter();

    Client::getInstance()->start();
	// 认证是否成功
    HANDLER(authentication) = Client::handler([this](net_pkg *pkg) {
		if (pkg->arg1) { // 成功，进入房间列表
            log("[Auth success]");
			Director::getInstance()->pushScene(RoomListScene::createScene());
		}
        else { // 失败，弹对话框
            //log("[Auth failure]: %s", pkg->data);
            Dialog::getInstance()->Popup_t(this, " 错误 ", pkg->data);
        }
        HANDLER(authentication) = nullptr; // 置空，防止重复调用
    });
}

void RoleScene::onStartClick(Ref *ref) {
    if(!Client::getInstance()->isConnected()) {
        //log("[Connect] server failure.");
        Dialog::getInstance()->Popup_t(this, " 错误 ", " 连接服务器失败 ");
        return;
    }
	if (m_editName->getString().empty()) { // 昵称不能为空
        //log("[Error:]nick name is empty");
        Dialog::getInstance()->Popup_t(this, " 错误 ", " 昵称不能为空!! ");
		return;
	}
    Client::getInstance()->sendMsg(authentication,
                                   (msg_arg)m_pageSprite->getCurPageIndex(),
                                   m_editName->getString().c_str());
}

void RoleScene::onLeaveClick(Ref *ref) {
    Director::getInstance()->popScene();
}
