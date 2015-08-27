#include "WomanSoldier.h"
#include "AppDelegate.h"

bool WomanSoldier::init() {
	m_sprite = Sprite3D::create("test.c3b");
	m_sprite->setRotation3D(Vec3(180.f , 180.f, 180.f));
	m_sprite->setScale(2.f);

	m_anim_walk = Animation3D::create("test.c3b");
	m_anim_throw = Animation3D::create("throw.c3b");

	addChild(m_sprite);
    return true;
}

void WomanSoldier::action_throw() {
	auto animate = Animate3D::createWithFrames(m_anim_throw, 0, 50, 50.f);
	m_sprite->runAction(animate);
}

void WomanSoldier::action_walk() {
	auto animate = Animate3D::createWithFrames(m_anim_walk, 35, 65, 50.f);
	//auto action = RepeatForever::create(animate);
	m_sprite->runAction(animate);
}
