#ifndef __ROOM_H__
#define __ROOM_H__

#include <QHash>
#include <QString>
#include <QTcpSocket>

#include "handler.h"

class Room {
public:
    static QHash<QString, Room *> g_room;
    static bool exist(char *name);
    static QList<QString> list();

    Room(char *name);
    ~Room();
    
    inline char *name() {
        return m_name;
    }
    inline char *error() {
        return m_err;
    }

    bool add(Member *meb); //添加成员
    bool remove(Member *meb); //移除成员

    void broadMembers(); //广播成员列表

    template<typename ... ARGS>
    void broadcast(net_pkg *pkg, ARGS ... args) { //广播消息
        //遍历各个位置，有成员则发送广播消息
        for(int i = 0; i < MAX_ROOM_MEMBERS; i++) {
            Member *m = m_members[i];
            if(m) {
                m->handler()->Reply(pkg, args...);
            }
        }
    }

protected:
    char m_name[MAX_ROOM_NAME_LEN]; //房间名称
    char *m_err = nullptr; //上一个错误
    Member *m_members[MAX_ROOM_MEMBERS] = { nullptr }; //成员列表
    //Member *m_master = nullptr; //房间主人
};

#endif
