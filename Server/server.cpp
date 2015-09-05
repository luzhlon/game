#include "handler.h"
#include "server.h"

//一个连接处理线程分配一个处理器
ConnectThread::ConnectThread(QTcpSocket *sock) {
    m_sock = sock;
}

void ConnectThread::run() {
    MsgHandler handler(m_sock);
    handler.loopHandle();
}

void WorkThread::run() {
    qDebug() << "Begain working...\n";
    //新建一个QTcpServer并监听4321端口
    m_server = new QTcpServer();
    m_server->listen(QHostAddress::Any, 4321);
    //
    while(true) {
        if(m_server->waitForNewConnection(-1)) { //waitXXX 阻塞等待
            qDebug() << "New client:";
            QTcpSocket *sock = m_server->nextPendingConnection();
            qDebug() << sock->peerAddress().toIPv4Address() << "\n";

            auto thread = new ConnectThread(sock);
            thread->start(); //运行连接处理线程
        } else { //accept failure
            qDebug() << "Quit listening.\n";
            break;
        }
    }
}

void startWork() {
    static WorkThread* thread = nullptr;
    if(thread) return; //保证只有一个工作线程
    thread = new WorkThread();
    thread->start();
}
