#ifndef __MESSAGE_H__
#define __MESSAGE_H__

//包的最大长度
#define MAX_PKG_LENGTH 10240
//房间成员昵称名字的最大长度
#define MAX_MEMBER_NAME_LEN 16
//房间名字的最大长度
#define MAX_ROOM_NAME_LEN 32
//房间最大成员数
#define MAX_ROOM_MEMBERS 6

struct mini_net_pkg {
    unsigned short len; //数据包长度
    unsigned short msg; //消息
    int arg1; //参数1
    int arg2; //参数2
}; //net_pkg - data[MAX_PKG_LENGTH]

struct net_pkg : mini_net_pkg {
    char data[MAX_PKG_LENGTH]; //额外的数据
}; //数据包

struct room_member {
    unsigned char m_ready = 0;
    unsigned char m_room_id;
    unsigned short m_role_id;
    char m_name[MAX_MEMBER_NAME_LEN];
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
        room_members,
        start_game,
        toggle_ready,
        toggle_team,

        Max_number //消息的最大数目
    }; //主连接消息
}

#endif
