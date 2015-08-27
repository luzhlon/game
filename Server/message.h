#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <iostream>

//包的最大长度
#define MAX_PKG_LENGTH 10240
//房间成员昵称名字的最大长度
#define MAX_MEMBER_NAME_LEN 16
//房间名字的最大长度
#define MAX_ROOM_NAME_LEN 32
//房间最大成员数
#define MAX_ROOM_MEMBERS 6

typedef int msg_arg;
typedef unsigned short msg_len;
typedef unsigned short msg_msg;

struct mini_net_pkg {
    msg_len len; //数据包长度
    msg_msg msg; //消息
    msg_arg arg1; //参数1
    msg_arg arg2; //参数2
}; //net_pkg - data[MAX_PKG_LENGTH]

struct net_pkg : mini_net_pkg {
    char data[MAX_PKG_LENGTH]; //额外的数据
}; //数据包

#define MEMBER_FLAG_READY (0x01)
#define MEMBER_FLAG_TEAM (0x02)

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
}; //房间成员

struct room_state {
    char name[MAX_ROOM_NAME_LEN];
    room_member members[MAX_ROOM_MEMBERS];
}; //

//最小的包长度
#define NET_PKG_SIZE (sizeof(mini_net_pkg))
//只有一个参数的包的长度
#define NET_PKG_SIZE_1 (sizeof(unsigned short) * 2 + sizeof(int) * 1)
//有两个参数的包的长度
#define NET_PKG_SIZE_2 (sizeof(unsigned short) * 2 + sizeof(int) * 2)

//消息定义
namespace MESSAGE {
    enum {
        authentication = 0,
        room_list, //房间列表
        create_room, //创建房间
        join_room , //加入房间
        quit_room , //退出房间
        room_members, //房间成员信息
        start_game, //开始游戏
        set_ready, //设置准备状态
        set_team, //设置队伍

        Max_number //消息的最大数目
    }; //主连接消息
}

static const char *g_check_name(char *name) {
    std::string str = name;
    if(str.empty()) {
        return "Empty name";
    }
    if((str.find(' ') != std::string::npos) ||
            (str.find("　") != std::string::npos)) {
        return "Contain space character";
    }
    return nullptr;
}

#endif
