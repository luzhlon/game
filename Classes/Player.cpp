#include "Player.h"

Soldier::Type Player::s_type = Soldier::SOLDIER_TYPE_WOMAN;

Soldier *g_self = nullptr;

Skill *Player::skill_kick = nullptr;
Skill *Player::skill_boxing = nullptr;
Skill *Player::skill_special = nullptr;

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
    show_circle();
    //addChild(_soldier);
}

void Player::show_circle(bool show) {
    static Sprite3D *circle3d = nullptr;
    if (!circle3d) {
        circle3d = Sprite3D::create();
        auto circle = Sprite::create("image/circle.png");

        circle3d->setScale(0.3f);
        circle3d->addChild(circle);
        circle->runAction(RepeatForever::create(RotateBy::create(3.5f, Vec3(0.f, 0.f, 360.f))));

        circle3d->setRotation3D(Vec3(-90.f, 0.f, 0.f));

        _soldier->add_thing(circle3d);
    }
    circle3d->setVisible(show);
}

void Player::draw_circle(float radius, Color4F& color) {
    if (!_draw) {
        _draw = DrawNode::create();
        _soldier->add_thing(_draw);
    }
    _draw->clear();
    _draw->drawDot(Vec2(0, 0), radius, color);
    auto clr = color;
    clr.a = 0;
    _draw->drawCircle(Vec2(0, 0), radius, CC_DEGREES_TO_RADIANS(90.f), 50, false, clr);
    _draw->setRotation3D(Vec3(90, 0, 0));
    //_draw->setGlobalZOrder(1);
}
