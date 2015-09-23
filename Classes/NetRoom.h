#include <string>
#include "cocos2d.h"
#include "../Server/message.h"
#include "Soldier.h"

USING_NS_CC;

struct Skill;

class GoodsBase;

namespace NetRoom {
    extern int         _self_id;
    extern int         _master_id;
    extern bool        _game_over;
    extern std::string _room_name;
    extern room_member _members[MAX_ROOM_MEMBERS];
    extern net_pkg     _pkg;
    extern Vec2        _house_pos;  // 营地位置

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
    void set_grass(int count, int room_id = _self_id);
    void add_goods(GoodsBase *);
    void dec_goods(int index);

    int  get_team_grass(int room_id = _self_id);

    void declare_win(int team);

    // 获得附近的敌人的ID，没有返回-1
    int get_near_enemy(float distance = 50.f);

    void do_skill(Skill *);
}