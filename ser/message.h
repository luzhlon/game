﻿#ifndef __MESSAGE_H__
#define __MESSAGE_H__

typedef int msg_arg;
typedef unsigned short msg_len;
typedef unsigned short msg_msg;

//包的最大长度
#define MAX_PKG_LENGTH 10240

struct mini_net_pkg {
    msg_len len;
    msg_msg msg;
    msg_arg arg1;
    msg_arg arg2;
};

struct net_pkg : mini_net_pkg {
    char data[MAX_PKG_LENGTH]; //额外的数据
}; //数据包

//房间成员昵称名字的最大长度
#define MAX_MEMBER_NAME_LEN 16
//房间名字的最大长度
#define MAX_ROOM_NAME_LEN 32
//房间最大成员数
#define MAX_ROOM_MEMBERS 6

#define MEMBER_FLAG_READY (0x01)
#define MEMBER_FLAG_TEAM (0x02)
#define MEMBER_FLAG_EMPTY (0x04) //空，此位置没有成员

struct room_member {
    unsigned char m_flag = 0;
    unsigned char m_room_id;
    unsigned short m_role_id;
    char m_name[MAX_MEMBER_NAME_LEN];

    inline void set_ready_0() {
        m_flag &= ~MEMBER_FLAG_READY;
    }
    inline void set_ready_1() {
        m_flag |= MEMBER_FLAG_READY;
    }
    inline void set_team_0() {
        m_flag &= ~MEMBER_FLAG_TEAM;
    }
    inline void set_team_1() {
        m_flag |= MEMBER_FLAG_TEAM;
    }
    inline bool get_ready() {
        return static_cast<bool>(m_flag & MEMBER_FLAG_READY);
    }
    inline bool get_team() {
        return static_cast<bool>(m_flag & MEMBER_FLAG_TEAM);
    }
    inline bool is_empty() {
        return m_flag & MEMBER_FLAG_EMPTY;
    }
    inline bool set_empty() {
        return m_flag |= MEMBER_FLAG_EMPTY;
    }
}; //房间成员

//最小的包长度
#define NET_PKG_SIZE (sizeof(mini_net_pkg))  // 12
//只有一个参数的包的长度
#define NET_PKG_SIZE_1 (sizeof(unsigned short) * 2 + sizeof(int) * 1)
//有两个参数的包的长度
#define NET_PKG_SIZE_2 (sizeof(unsigned short) * 2 + sizeof(int) * 2)

//消息定义
namespace MESSAGE {
    enum {
        authentication = 0,
        room_list, //房间列表      1
        create_room, //创建房间    2
        join_room , //加入房间     3
        quit_room , //退出房间     4
        room_members, //房间成员信息    5
        start_game, //开始游戏     6
        set_ready, //设置准备状态    7
        set_team, //设置队伍          8

        update_state,               // 9
        update_position,  //          10
        update_angle, //           11
        update_speed,   //           12
        update_blood,      //          13

        action_move,   //           14
        action_stop,   //           15

        do_skill,  //                16
        on_attacked,  //             17

        update_grass, //           18
        add_goods,     //       19
        dec_goods,        //     20

        set_master,     // 21
        game_over,       // 22

        Max_number //消息的最大数目
    }; //主连接消息
}

#include <functional>
#include <string.h>

class PackageCache {
public:
    typedef std::function<void (char*, int)> Callback;

    PackageCache(Callback cb, int length = 64 * 1024)
        : i_length(length) {
        m_cb = cb;
        m_buf = new char[length];
    }
    ~PackageCache() {
        delete m_buf;
    }


    void write_data(char *buf, int size);

    inline void on_read_pkg(Callback cb) {
        m_cb = cb;
    }

    inline int delta() { // 未处理的数据长度
        return i_write - i_read;
    }

    inline void lock() { while(b_lock); // 先等待解锁
                    b_lock = true; }
    inline void unlock() { b_lock = false; }

private:
    int i_read = 0;
    int i_write = 0;
    int i_length = 0;
    Callback m_cb = nullptr;
    char *m_buf = nullptr;
    bool b_lock = false;
    bool b_last_pkg_end = true;

    inline void write(char *buf, int size) {
        memcpy(&m_buf[i_write], buf, size);
        i_write += size;
    }
};

#endif //
