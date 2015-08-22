#include <QThread>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qtcpserver.h>

class MsgHandler;

//连接处理线程
class ConnectThread : public QThread {
public:
    ConnectThread(QTcpSocket *);
    void run() Q_DECL_OVERRIDE;

private:
    QTcpSocket *m_sock; //socket
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
