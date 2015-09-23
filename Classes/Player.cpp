#include "Player.h"
#include "Client.h"
#include "World.h"
#include "GameScene.h"
#include "NetRoom.h"

Soldier::Type Player::s_type = Soldier::SOLDIER_TYPE_WOMAN;

Soldier *g_self = nullptr;

Skill *Player::skill_kick = nullptr;
Skill *Player::skill_boxing = nullptr;
Skill *Player::skill_special = nullptr;
Skill *Player::skill_speed = nullptr;

const float Player::s_full_magic = 100.f;

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

    skill_kick = self->get_skill(Skill::SKILL_KICK);
    skill_kick->_button = GameScene::s_btn_kick;
    skill_boxing = self->get_skill(Skill::SKILL_BOXING);
    skill_boxing->_button = GameScene::s_btn_boxing;
    skill_special = self->get_skill(Skill::SKILL_SPECIAL);
    skill_special->_button = GameScene::s_btn_special;
    skill_speed = self->get_skill(Skill::SKILL_SPEED);
    skill_speed->_button = GameScene::s_btn_speed;

    self->setCameraFollowed();
    show_circle();

    self->onDeath([this](Soldier *sol) {
        NetRoom::set_state(Soldier::SOLDIER_STATE_DEATH);
        //scheduleOnce(schedule_selector(Player::revive), 8.f); // 8秒复活
        GameScene::Instance->begain_progress(20.f, std::bind(&Player::revive, this));
    });

    schedule(schedule_selector(Player::update_per_second), 1.f);
}

bool Player::do_skill(Skill *skill) {
    if(skill->_magic_dec != 0.f)
        add_magic(skill->_magic_dec);

    NetRoom::do_skill(skill);
    return true;
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
        auto pos = circle3d->getPosition3D();
        pos.y += 2;
        circle3d->setPosition3D(pos);

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

void Player::set_magic(float magic) {
    _magic = magic;
    if (_magic > s_full_magic) _magic = s_full_magic;
    if (_magic < 0.f) _magic = 0.f;

    GameScene::s_load_magic->setPercent(_magic / s_full_magic * 100.f);
}

void Player::onSkillClicked(Ref *ref) {

    if (g_self->death()) return; // 死亡状态

    auto btn = static_cast<Button *>(ref);
    Skill *skill;
    do{
        if (btn == GameScene::s_btn_boxing) {
            skill = Player::skill_boxing;
            break;
        }
        if (btn == GameScene::s_btn_kick) {
            skill = Player::skill_kick;
            break;
        }
        if (btn == GameScene::s_btn_special) {
            skill = Player::skill_special;
            break;
        }
        if (btn == GameScene::s_btn_speed) {
            skill = Player::skill_speed;
            break;
        }
    } while (false);

    //技能正在冷却
    if (skill->is_cooling()) return;

    skill->cooling();

    Player::getInstance()->do_skill(skill);
};

void Player::update_per_second(float dt) {
    do {
    if (Soldier::SOLDIER_STATE_IDLE == _soldier->_state) {
        if (_magic < s_full_magic) add_magic(1);     // 回魔

        if (_soldier->_blood < _soldier->_full_blood) 
            NetRoom::set_blood(_soldier->_blood + _soldier->_vitality); // 回血
        break;
    }
    if (Soldier::SOLDIER_STATE_MOVE == _soldier->_state) { // 每秒同步一下位置, 
        auto p3 = _soldier->getPosition3D();
        Vec2 pos(p3.x, p3.z);
        NetRoom::set_position(pos);
        break;
    }
    } while (false);
}

void Player::revive() {
    _soldier->_state = Soldier::SOLDIER_STATE_IDLE;
    NetRoom::set_position(NetRoom::_house_pos);
    NetRoom::set_blood(_soldier->_full_blood * 0.8f);
    NetRoom::set_state(Soldier::SOLDIER_STATE_REVIVE);
}

void Player::on_get_goods(Goods *good) {
    switch (good->type) {
    case Goods::GRASS:
        NetRoom::set_grass(_soldier->grass() + s_good.count);
        break;
    }

    // remove this good from world
    NetRoom::dec_goods(good);
}

void Player::on_pick_goods() {
    static Goods s_good;

    auto pos3 = g_self->getPosition3D();
    Vec2 pos(pos3.x, pos3.z);
    if (!g_world->get_goods(pos, &s_good)) return; // pick failure

    g_player->on_get_goods(&good);
    GameScene::Instance->begain_progress(3.f, [this](float dt, bool end) {
        if (!end) { // 读条未结束时
            //必须处于IDLE状态
            if (_soldier->state() != Soldier::SOLDIER_STATE_IDLE)
                GameScene::Instance->break_progress();

            on_get_goods(&s_good);
        }
    });
}

extern Soldier *g_soldiers[MAX_ROOM_MEMBERS];

void Player::set_grass(int count) {
    _soldier->grass(count);
    GameScene::set_score(count);
    // 统计两队Grass
    int score[2] = { 0 };
    for (int i = 0; i < MAX_ROOM_MEMBERS; i += 1) {
        auto sol = g_soldiers[i];
        if (sol) score[i % 2] += sol->grass();
    }
    GameScene::set_score_red(score[0]);
    GameScene::set_score_blue(score[1]);
    //判断获胜
    /*   */
}
