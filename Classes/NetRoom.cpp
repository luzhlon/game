#include "NetRoom.h"
#include "GameScene.h"
#include "World.h"
#include "Skill.h"
#include "Player.h"
#include "Client.h"
#include "Soldier.h"

using namespace NetRoom;

extern World *g_world;
extern Player *g_player;
extern Client *g_client;
extern Soldier *g_soldiers[MAX_ROOM_MEMBERS];

namespace NetRoom {
    int         _self_id;
    std::string _room_name;
    room_member _members[MAX_ROOM_MEMBERS];
    net_pkg     _pkg;
    Vec2        _house_pos;
}

void NetRoom::init() {
    register_handlers();
    create_soldiers();

    g_world->on_gen_goods([](Goods *goods) {
        add_goods(goods);
    });
}

void NetRoom::register_handlers() {
    HANDLER(update_state) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->switch_state((Soldier::State)pkg->arg2);
        //sol->target_point(*(Vec3 *)(pkg->data));
        //sol->addState((Soldier::State)pkg->arg2);
    });
    HANDLER(update_blood) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->set_blood(*(float *)pkg->data);
    });
    HANDLER(update_speed) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->speed(*(float *)pkg->data);
    });
    HANDLER(update_angle) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->angle(*(float *)pkg->data);
    });
    HANDLER(update_position) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        auto v2 = (Vec2 *)(pkg->data);

        g_world->set_position(sol, *v2);

        if(pkg->arg1 == _self_id) 
            GameScene::set_small_direction(sol->angle()); // 小地图方向箭头
    });
    HANDLER(action_move) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->move_to(*(Vec2 *)pkg->data);

        if(pkg->arg1 == _self_id) 
            GameScene::set_small_direction(sol->angle()); // 小地图方向箭头
    });
    HANDLER(action_stop) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->move_stop();
    });
    HANDLER(do_skill) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->do_skill((SkillBase *)pkg->data);
    });
    HANDLER(on_attacked) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1]; //Soldier be attacked
        SkillBase *skill = (SkillBase *)pkg->data;

        if (sol->death()) return;

        sol->on_skill(skill);
        sol->show_blood_decline(skill->_blood);

        if (pkg->arg1 == _self_id) {
            set_blood(sol->blood()); // 在房间里更新自己的血量
        }
    });
    HANDLER(update_grass) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1]; 
        if (pkg->arg1 == _self_id) g_player->set_grass(pkg->arg2);
        else  sol->grass(pkg->arg2);
    });
    HANDLER(add_goods) = Client::handler([](net_pkg *pkg) {
        g_world->add_goods((Goods *)pkg->data);
    });
    HANDLER(dec_goods) = Client::handler([](net_pkg *pkg) {
        g_world->dec_goods((Goods *)pkg->data);
    });
}

void NetRoom::create_soldiers() {
    for (int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        room_member *meb = &NetRoom::_members[i];
        if (meb->is_empty()) {
            g_soldiers[i] = nullptr;
            continue;
        }
        g_soldiers[i] = Soldier::create(meb); // 创建
        g_soldiers[i]->name_text()->setString(meb->m_name); // 玩家名称
        g_soldiers[i]->name_text()->setColor( i % 2 ?
                                                Color3B(255, 0, 0) :  // 蓝队
                                                Color3B(0, 0, 255));  // 红队
        g_world->add_thing(g_soldiers[i]); // 添加到世界中
        //g_world->set_position(g_soldiers[i], i % 2 ? 
                                            //Vec2(103.f, -503.f) :
                                            //Vec2(246.f, 484.f));
    }

    if (_self_id % 2) {  // 蓝队
        _house_pos = Vec2(103.f, -503.f);
    } else { // 红队
        _house_pos = Vec2(246.f, 484.f);
    }

    auto self = g_soldiers[NetRoom::_self_id];
    g_player = Player::getInstance(self); // 初始化Player
    g_world->addChild(g_player); // 加到景中才能触发定时器
    set_grass(self->grass());
}

void NetRoom::action_move(Vec2& pos) {
    if (g_soldiers[_self_id]->death()) return;

    _pkg.msg = MESSAGE::action_move;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, &pos, sizeof(pos));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(pos));
}

void NetRoom::action_stop() {
    g_client->sendMsg(MESSAGE::action_stop, _self_id);
}

void NetRoom::set_angle(float angle) {
    _pkg.msg = MESSAGE::update_angle;
    _pkg.arg1 = _self_id;
    *(float *)_pkg.data = angle;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(float));
}

void NetRoom::set_blood(float blood) {
    _pkg.msg = MESSAGE::update_blood;
    _pkg.arg1 = _self_id;
    *(float *)_pkg.data = blood;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(blood));
}

void NetRoom::set_speed(float speed) {
    _pkg.msg = MESSAGE::update_blood;
    _pkg.arg1 = _self_id;
    *(float *)_pkg.data = speed;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(speed));
}

void NetRoom::set_position(Vec2& pos) {
    _pkg.msg = MESSAGE::update_position;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, &pos, sizeof(pos));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(pos));
}

void NetRoom::do_skill(Skill *skill) {
    _pkg.msg = MESSAGE::do_skill;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, skill, sizeof(SkillBase));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(SkillBase));

    _pkg.msg = MESSAGE::on_attacked;
    //从对方队伍里寻找离自己近的人
    for (int i = (_self_id % 2 + 1) % 2; i < MAX_ROOM_MEMBERS; i += 2) {
        auto s = g_soldiers[i];
        if (!s) continue;
        if (s->member()->is_empty()) continue;

        if (s->death()) continue; // 对方已经死亡

        auto self = g_soldiers[_self_id];
        auto delta3 = s->getPosition3D() - self->getPosition3D();
        Vec2 delta(delta3.x, delta3.z);

        auto distance = delta.length(); // 相差距离
        auto angle = CC_RADIANS_TO_DEGREES(delta.getAngle()) - self->angle(); 
        angle = fabs(angle); // 相差角度

        if (distance <= skill->_distance && angle <= skill->_angle) { //在技能的攻击范围内
            _pkg.arg1 = i;
            g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(SkillBase)); // 发动攻击
        }
    }
}

void NetRoom::set_state(Soldier::State state) {
    _pkg.msg = MESSAGE::update_state;
    _pkg.arg1 = _self_id;
    _pkg.arg2 = state;
    //*(float *)_pkg.data = blood;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg));
}

void NetRoom::set_grass(int count) {
    _pkg.msg = MESSAGE::update_grass;
    _pkg.arg1 = _self_id;
    _pkg.arg2 = count;
    //*(float *)_pkg.data = blood;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg));
}

void NetRoom::add_goods(Goods *good) {
    _pkg.msg = MESSAGE::add_goods;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, good, sizeof(Goods));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(Goods));
}
void NetRoom::dec_goods(Goods *good) {
    _pkg.msg = MESSAGE::dec_goods;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, good, sizeof(Goods));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(Goods));
}
