#ifndef HANDLER_H
#define HANDLER_H

#include <QTcpSocket>
#include "message.h"
#include "member.h"

class WorkThread;

class MsgHandler {
public:
    typedef void (*Handler)(MsgHandler*, net_pkg *); //

    MsgHandler();
    ~MsgHandler();

    void handle(net_pkg *p = 0); //总的 处理消息
    void loopHandle(); //开始循环处理用户消息
    inline bool endHandle() { //结束处理？
        return m_end;
    }
    inline void endHandle(bool end) { //结束处理？
        m_end = end;
    }

    bool Recv(char *buf = 0); //接收用户消息
    int _Reply(net_pkg *p, int size); //回复用户
    inline int Reply(net_pkg *p) {
        return _Reply(p, NET_PKG_SIZE_1);
    }
    inline int Reply(net_pkg *p, int arg1) {
        p->arg1 = arg1;
        return _Reply(p, NET_PKG_SIZE_1);
    }
    inline int Reply(net_pkg *p, int arg1, int arg2) {
        p->arg1 = arg1;
        p->arg2 = arg2;
        return _Reply(p, NET_PKG_SIZE_2);
    }
    inline int Reply(net_pkg *p, void *data, int data_size) {
        memcpy(p->data, data, data_size);
        return _Reply(p, NET_PKG_SIZE + data_size);
    }
    inline int Reply(net_pkg *p, char *str) { //回复用户，跟一个字符串
        return Reply(p, (void *)str, strlen(str) + 1);
    }
    inline int Reply(net_pkg *p, int arg1, char *str) { //回复用户，跟一个字符串
        p->arg1 = arg1;
        return Reply(p, str);
    }

    inline QTcpSocket *socket() {
        return m_socket;
    }

    inline Member* member() {
        return m_member;
    }

private:
    friend class WorkThread;
    friend class ConnectThread;

    QTcpSocket *m_socket; //此处理器持有的socket
    Handler *m_handlers = nullptr; //处理器向量表
    Member * m_member; //此处理器对应的用户
    //int      m_maxNumber = 0;  //消息的最大数目
    bool     m_end = false; //表明是否要结束处理
    net_pkg  m_buf; //接收的数据包缓冲区
};

#define HANDLER(MSG) m_handlers[MESSAGE::MSG] = [](MsgHandler *self, net_pkg *pkg)

#endif // HANDLER_H
