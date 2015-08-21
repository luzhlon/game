#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <thread>
#include <condition_variable>
#include <functional>
#include "../Server/message.h"
#include "cocos2d.h"
#include "ODSocket.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "windows.h"
#define sleep Sleep
#else
#include "unistd.h"
#define sleep usleep
#endif

#define MAX_BLOCK_MSG_NUM 16
#define INCREASE(num)  num = (num == MAX_BLOCK_MSG_NUM) ? 0 : num + 1
#define HANDLER(msg) Client::s_handlers[MESSAGE::msg]

class Client : public cocos2d::Ref
{
public:
    typedef std::function<void (net_pkg *)> handler;
    static Client *getInstance();
    static char *s_serverIP;
    static int s_serverPort;
    static handler s_handlers[MESSAGE::Max_number];
    static net_pkg s_pkg;

    void dispatchMsg(float );

    bool send(char *, int);
    inline bool sendMsg(net_pkg *pkg, int size) {
		pkg->len = size;
        return send((char*)pkg, size);
    }
    inline void copy_data(void *data, int size) {
        if(data)
            memcpy(s_pkg.data, data, size);
    }
    inline bool sendMsg(unsigned short msg, int arg1, char *str) {
        return sendMsg(msg, arg1, strlen(str) + 1, str);
    }
    inline bool sendMsg(unsigned short msg, int data_size = 0, void *data = nullptr) {
        copy_data(data, data_size);
        s_pkg.msg = msg;
        return sendMsg(&s_pkg, data ? NET_PKG_SIZE + data_size : NET_PKG_SIZE_2);
    }
	inline bool sendMsg(unsigned short msg, int arg1,
		int data_size = 0, void *data = nullptr) {
		copy_data(data, data_size);
		s_pkg.arg1 = arg1;
		s_pkg.msg = msg;
		return sendMsg(&s_pkg, data ? NET_PKG_SIZE + data_size : NET_PKG_SIZE_2);
	}
    inline bool sendMsg(unsigned short msg, const char *str) {
        return sendMsg(msg, strlen(str) + 1, (void *)str);
    }
    inline bool sendMsg(unsigned short msg, char *str) {
        return sendMsg(msg, strlen(str) + 1, (void *)str);
    }

    inline net_pkg *getMsg() {
        return m_pkg;
    }
    inline bool isConnected() {
        return m_connect;
    }

    bool connectSever();

    void threadRecv();
    void start(); //in a new thread
private:
    static Client *s_client;
    Client();
    std::thread *m_thread = nullptr;
    ODSocket m_sock;
    net_pkg *m_pkg = nullptr;
    bool m_handled = true;
    bool m_connect = false;
    /*
    net_pkg *m_msgs[MAX_BLOCK_MSG_NUM] = { nullptr };
    int m_get_ptr = 0;
    int m_put_ptr = 0; // */
};

#endif // __CLIENT_H__