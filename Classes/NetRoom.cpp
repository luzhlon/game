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
}

void NetRoom::init() {
    register_handlers();
    create_soldiers();
}

void NetRoom::register_handlers() {
    HANDLER(update_state) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->target_point(*(Vec3 *)(pkg->data));
        sol->addState((Soldier::State)pkg->arg2);
    });
    HANDLER(update_blood) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->set_blood(*(float *)pkg->data);
    });
    HANDLER(update_speed) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->speed(*(float *)pkg->data);
    });
    HANDLER(update_position) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->setPosition3D(*(Vec3 *)(pkg->data));
    });
    HANDLER(action_move) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->switch_state(Soldier::SOLDIER_STATE_MOVE, pkg->data);
        GameScene::set_small_direction(sol->get_rotation()); // 小地图方向箭头
    });
    HANDLER(action_stop) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->switch_state(Soldier::SOLDIER_STATE_IDLE);
        //sol->move_stop();
    });
    HANDLER(do_skill) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->do_skill((SkillBase *)pkg->data);
    });
    HANDLER(on_attacked) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1]; //Soldier be attacked
        SkillBase *skill = (SkillBase *)pkg->data;
        sol->be_injured(skill);
        sol->show_blood_decline(skill->_dec_blood);
    });
}

void NetRoom::create_soldiers() {
    for (int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        room_member *meb = &NetRoom::_members[i];
        if (meb->is_empty()) continue;
        g_soldiers[i] = Soldier::create(meb); // 创建
        g_soldiers[i]->name_text()->setString(meb->m_name); // 玩家名称
        g_soldiers[i]->name_text()->setColor( i % 2 ?
                                                Color3B(255, 0, 0) :  // 红队
                                                Color3B(0, 0, 255));  // 蓝队
        g_world->add_thing(g_soldiers[i]); // 添加到世界中
    }

    g_player = Player::getInstance(g_soldiers[NetRoom::_self_id]); // 初始化Player
}

void NetRoom::action_move(Vec3& pos) {
    _pkg.msg = MESSAGE::action_move;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, &pos, sizeof(pos));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(pos));
}

void NetRoom::action_stop() {
    g_client->sendMsg(MESSAGE::action_stop, _self_id);
}

void NetRoom::set_position(Vec3& pos) {
    _pkg.msg = MESSAGE::update_position;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, &pos, sizeof(pos));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(pos));
}

void NetRoom::do_skill(SkillBase *skill) {
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

        auto self = g_soldiers[_self_id];
        if (self->getPosition3D().distance(s->getPosition3D()) < skill->_dec_distance) {
            _pkg.arg1 = i;
            g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(SkillBase)); // 发动攻击
        }
    }
}
