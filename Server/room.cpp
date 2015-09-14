#include <QDebug>
#include "member.h"
#include "room.h"
#include "handler.h"

QHash<QString, Room *> Room::g_room;

Room::Room(char *name) {
    if(strlen(name) >= MAX_ROOM_NAME_LEN) {
        m_err = "room name is too long";
        return;
    } //房间名称太长
    if(Room::getRoom(name)) { //房间已经存在
        m_err = "room existed";
        return;
    }
    m_err = g_check_name(name);
    if(m_err) return;
    strcpy(m_name, name); //
    g_room[m_name] = this; //从哈希表中加入自己

    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) m_members[i] = nullptr; //初始化成员为空
}

Room::~Room() {
    g_room[m_name] = nullptr; //从哈希表中删除自己
}

bool Room::add(Member *meb) {
    int i = 0;
    for(Member *m = nullptr; //找一个空位置加入成员
        i < MAX_ROOM_MEMBERS; i++) {
        m = m_members[i];
        if(!m) {  //success
            m_members[i] = meb;
            meb->m_room_id = i;
            meb->m_room = this; //设置成员的所属房间为自己
            qDebug() << "Room:" << m_name << "," << meb->m_name << "joined.";
            broadMembers(); //广播房间中的成员信息
            return true;
        }
    }
    //房间已经满员了
    m_err = "room is full";
    return false;
}

bool Room::remove(Member *meb) {
    auto r_id = for_member_id(meb);
    if(error()) {
        m_err = "no this member";
        return false;
    }
    m_members[r_id] = nullptr;
    broadMembers(); //广播房间中的成员信息
    qDebug() << "Room:" << m_name << "," << meb->m_name << " quited.";
    return true;
}

void Room::broadMembers() {
    net_pkg pkg;
    room_member meb[MAX_ROOM_MEMBERS];

    pkg.msg = MESSAGE::room_members;
    pkg.arg1 = 0; //房间成员数

    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        auto m = m_members[i] ;
        if(m) {
            pkg.arg1 ++; //成员数
            memcpy(&meb[i], m, sizeof(room_member));
            strcpy(meb[i].m_name, m->m_name); // 成员的名字
        } else {
            memset(&meb[i], 0, sizeof(room_member)); //此位置清零
        }
    }
    size_t size = sizeof(room_member) * MAX_ROOM_MEMBERS;
    broadcast(&pkg, meb, size);
}

int Room::for_member_id(Member *meb) {
    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        if(m_members[i] == meb) {
            m_err = nullptr;
            return i;
        }
    }
    m_err = "no this member";
}

void Room::checkAllReady() {
    bool all = true;
    for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        auto m = m_members[i];
        if(m) {
            all = all && m->get_ready();
        }
    }
    mini_net_pkg pkg;
    if(all) { //enter game
        //broadcast(&pkg, MESSAGE::start_game);
    }
}

bool Room::setTeam(Member *meb, int team) {
    int r_id = for_member_id(meb);
    if(error()) {
        return false;
    }
    for(int i = team % 2; i < MAX_ROOM_MEMBERS; i += 2) {
        if(!m_members[i]) {
            m_members[i] = m_members[r_id];
            m_members[r_id] = nullptr;
            return true;
        }
    }
    m_err = "object team is full";
    return false;
}
