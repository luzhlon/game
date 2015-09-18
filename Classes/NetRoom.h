#include <string>
#include "cocos2d.h"
#include "../Server/message.h"

USING_NS_CC;

class SkillBase;

namespace NetRoom {
    extern int         _self_id;
    extern std::string _room_name;
    extern room_member _members[MAX_ROOM_MEMBERS];
    extern net_pkg     _pkg;

    void init();
    void create_soldiers();
    void register_handlers();

    void action_move(Vec3&);
    void action_stop();
    void set_position(Vec3&);

    void do_skill(SkillBase *);
}