#include <QThread>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qtcpserver.h>

class MsgHandler;

//连接处理线程
class ConnectThread : public QThread {
private:
    QTcpSocket *m_sock; //socket
    MsgHandler *m_handler; //消息处理器
public:
    ConnectThread(MsgHandler *handler);
    void run() Q_DECL_OVERRIDE;
};

//工作处理线程
class WorkThread : public QThread {
    Q_OBJECT
private:
    QTcpServer *m_server; //服务器
public:
    WorkThread() {}
    ~WorkThread() {}
    void run() Q_DECL_OVERRIDE;
};
