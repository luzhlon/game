#include "member.h"
#include "room.h"

QHash<QString, Member *> Member::g_member;

Member::Member(char *name, MsgHandler *handler) {
    if(strlen(name) >= MAX_MEMBER_NAME_LEN) {
        m_err = "nick name too long";
        return;
    }
    if(g_member[name]) {
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
    auto room = Room::g_room[room_name];
    if(!room) {
        m_err = "no this room";
        return false;
    }
    if(m_room) {
        m_err = "in a room now";
        return false;
    }
    if(!room->add(this)) {
        m_err = m_room->error();
        return false;
    }
    m_room = room;
    return true;
}

bool Member::quit_room() {
    if(!m_room) {
        m_err = "not in a room";
        return false;
    }
    if(!m_room->remove(this)) {
        m_err = m_room->error();
        return false;
    }
    m_room = nullptr;
    return true;
}

void Member::Ready() {
    m_ready = true;
    m_room->broadMembers();
}

void Member::unReady() {
    m_ready = false;
    m_room->broadMembers();
}
