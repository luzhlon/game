#include "RoleScene.h"
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
	
    return true;
}

bool RoleScene::loadPages() {
    auto node = CSLoader::createNode("role_page.csb");
    auto layout = static_cast<Layout*>
            (node->getChildByName("layout"));
    CC_ASSERT(layout);
    //layout->removeFromParent();
    for(int i = 0; i < Soldier::SoldierNumber; i++) {
        //Layout *layout;
        auto l = static_cast<Layout*>(layout->clone());
        auto s = Soldier::s_soldiers[i];
        auto size = m_pageSprite->getSize();
        l->setSize(size);
        l->setPosition(Vec2(size.width / 2.f, size.height / 2.f));

        size = l->getSize();
        s->setPosition(Vec2(size.width / 2.f, size.height / 2.f));

        l->addChild(s);
        l->removeFromParent();
        m_pageSprite->addPage(l);
    }
    return true;
}

void RoleScene::onEnter() {
	Layer::onEnter();
    CurScene(SCENE_ROLE);

    if(Soldier::loadAllSoldier()) {
        log("<LoadAll> success.");
        loadPages();
    } else {
        log("<LoadAll> failure.");
    }
    Client::getInstance()->start();
	//认证是否成功
    HANDLER(authentication) = Client::handler([this](net_pkg *pkg) {
        if(!IsCurScene(SCENE_ROLE)) return;
		if (pkg->arg1) { //成功，进入房间列表
            log("[Auth success]");
			Director::getInstance()->pushScene(RoomListScene::createScene());
		}
        else { //失败，弹对话框
            //log("[Auth failure]: %s", pkg->data);
            Dialog::getInstance()->Popup_t(this, "错误", "认证失败！");
        }
        HANDLER(authentication) = nullptr; //置空，防止重复调用
    });
}

void RoleScene::onStartClick(Ref *ref) {
    if(!Client::getInstance()->isConnected()) {
        log("[Connect] server failure.");
        return;
    }
	if (m_editName->getString().empty()) { //昵称不能为空
        log("[Error:]nick name is empty");
		return;
	}
    Client::getInstance()->sendMsg(authentication,
                                   (msg_arg)m_pageSprite->getCurPageIndex(),
                                   m_editName->getString().c_str());
}

void RoleScene::onLeaveClick(Ref *ref) {
    Director::getInstance()->popScene();
}
