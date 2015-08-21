#include "member.h"
#include "room.h"

QHash<QString, Member *> Member::g_member;

Member::Member(char *name, MsgHandler *handler) {
    //昵称太长
    if(strlen(name) >= MAX_MEMBER_NAME_LEN) {
        m_err = "nick name too long";
        return;
    }
    if(g_member[name]) { //此昵称已经存在
        m_err = "nick name existed";
        return;
    }
    strcpy(m_name, name);
    m_handler = handler;
    g_member[name] = this;
}

Member::~Member() {
    g_member[m_name] = nullptr;
}

bool Member::join_room(char *room_name) {
    auto room = Room::getRoom(room_name);
    if(!room) { //没有此房间
        m_err = "no this room";
        return false;
    }
    if(m_room) { //已经在一个房间里了
        m_err = "in a room now";
        return false;
    }
    if(!room->add(this)) {
        m_err = m_room->error();
        return false;
    }
    m_room = room; //设置自己的房间
    return true;
}

bool Member::quit_room() {
    if(!m_room) { //没有处在一个房间里
        m_err = "not in a room";
        return false;
    }
    if(!m_room->remove(this)) {
        m_err = m_room->error();
        return false;
    }
    m_room = nullptr; //把自己的房间置空
    return true;
}
