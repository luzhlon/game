#include "handler.h"
#include "server.h"

//一个连接处理线程分配一个处理器
ConnectThread::ConnectThread(MsgHandler *handler) {
    m_sock = handler->m_socket;
    m_handler = handler;
}

void ConnectThread::run() {
    /*
        m_sock->write("Hello client\n");
        m_sock->flush();
        // */
    m_handler->loopHandle();
    /*
    if(m_handler->authenticate()) {
        m_handler->loopHandle();//认证通过，进入事件处理循环
    } else {
        qDebug() << "Authenticate failure.";
    } // */
    delete m_handler;
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
            auto handler = new MsgHandler(); //分配一个处理器给连接处理线程
            handler->m_socket = sock;
            auto thread = new ConnectThread(handler); 
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
