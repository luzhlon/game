#include <QDebug>
#include "member.h"
#include "room.h"
#include "handler.h"

QHash<QString, Room *> Room::g_room;

bool Room::exist(char *name) {
    if(g_room[name]) {
        return true;
    } else {
        return false;
    }
}

QList<QString> Room::list() {
    return g_room.keys();
}

Room::Room(char *name) {
    if(strlen(name) >= MAX_ROOM_NAME_LEN) {
        m_err = "room name is too long";
        return;
    } //room name is too long
    if(Room::exist(name)) { //room existed
        m_err = "room existed";
        return;
    }
    strcpy(m_name, name); //
    g_room[m_name] = this; //从哈希表中加入自己
}

Room::~Room() {
    g_room[m_name] = nullptr; //从哈希表中删除自己
}

bool Room::add(Member *meb) {
    int i = 0;
    for(Member *m = nullptr; //找一个空位置加入成员
        i < MAX_ROOM_MEMBERS; i++) {
        m = m_members[i];
        if(!m) {
            m_members[i] = meb;
            meb->m_room_id = i;
            break;
        }
    }
    if(i == MAX_ROOM_MEMBERS) { //没有空位置
        m_err = "room is full";
        return false;
    }
    //Success
    meb->m_room = this; //设置成员的所属房间为自己
    qDebug() << "Room:" << m_name << "," << meb->m_name << "joined.";
    broadMembers(); //
    return true;
}

bool Room::remove(Member *meb) {
    bool ret = true;
    if(meb == m_members[meb->m_room_id]) {
        m_members[meb->m_room_id] = nullptr;
        qDebug() << "Room:" << m_name << "," << meb->m_name << " quited.";
    }
    else {
        m_err = "no this member";
        ret = false;
    }
    broadMembers(); //
    return ret;
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
            meb[i].m_ready = m->m_ready; //成员是否已准备好
            meb[i].m_role_id = m->m_role_id; //成员的角色ID
            strcpy(meb[i].m_name, m->m_name); // 成员的名字
        } else {
            meb[i].m_ready = 0;
            meb[i].m_role_id = 0;
        }
    }
    size_t size = sizeof(room_member) * MAX_ROOM_MEMBERS;
    //memcpy(pkg.data, meb, size);
    broadcast(&pkg, meb, size);
}
