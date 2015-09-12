#include "Player.h"

Soldier::Type Player::s_type = Soldier::SOLDIER_TYPE_WOMAN;

Soldier *g_self = nullptr;

Player *Player::getInstance(Soldier *soldier) {
    static Player *player = nullptr;
    if (!player) {
        CC_ASSERT(soldier);

        player = new Player(soldier);
    }
    return player;
}

Player::Player(Soldier *self) {
    _soldier = self;
    g_self = self;

    self->setCameraFollowed();
    showCircle();
    //addChild(_soldier);
}

void Player::showCircle(bool show) {
    static Sprite3D *circle3d = nullptr;
    if (!circle3d) {
        circle3d = Sprite3D::create();
        auto circle = Sprite::create("image/circle.png");

        circle3d->setScale(0.3f);
        circle3d->addChild(circle);
        circle->runAction(RepeatForever::create(RotateBy::create(3.5f, Vec3(0.f, 0.f, 360.f))));

        circle3d->setRotation3D(Vec3(90.f, 90.f, 0.f));

        //circle->setCameraMask(World::CAMERA_I);
        _soldier->addThing(circle3d);
    }
    circle3d->setVisible(show);
}
