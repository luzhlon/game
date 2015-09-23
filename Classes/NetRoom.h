#include <string>
#include "cocos2d.h"
#include "../Server/message.h"
#include "Soldier.h"

USING_NS_CC;

struct Skill;

class Goods;

namespace NetRoom {
    extern int         _self_id;
    extern std::string _room_name;
    extern room_member _members[MAX_ROOM_MEMBERS];
    extern net_pkg     _pkg;
    extern Vec2        _house_pos;  // ÓªµØÎ»ÖÃ

    void init();
    void create_soldiers();
    void register_handlers();

    void action_move(Vec2&);
    void action_stop();

    void set_state(Soldier::State state);
    void set_position(Vec2&);
    void set_angle(float angle);
    void set_blood(float blood);
    void set_speed(float speed);
    void set_grass(int count);
    void add_goods(Goods *);
    void dec_goods(Goods *);

    void do_skill(Skill *);
}