#include "Client.h"

int                     Client::s_serverPort = 4321;
std::list<net_pkg *>    Client::s_recv_list;
net_pkg                 Client::s_pkg;
Client::handler         Client::s_handlers[MESSAGE::Max_number];

Client::Client() {
    ODSocket::Init();
}

Client* Client::getInstance() {
    static Client *s_client = nullptr;
    if(!s_client) 
        s_client = new Client();
    return s_client;
}

extern char g_server_ip[32];

bool Client::connect_server() {
    if (!m_sock) m_sock = new ODSocket();
    m_sock->Create(AF_INET, SOCK_STREAM, 0);

    for(int i = 0; i < 5; i++) {
        if(m_sock->Connect(g_server_ip, s_serverPort)) {
            m_connect = true;
            return true;
        }
        sleep(1000);
    }
    //notify caller failure.
    m_connect = false;
    return false;
}

void Client::start() {
    if(isConnected()) return;
    m_thread = new std::thread(&Client::threadRecv, this);
    m_thread->detach();
    cocos2d::Director::getInstance()->getScheduler()->schedule(
                schedule_selector(Client::dispatchMsg), this, 0.02f, false);
}

void Client::dispatchMsg(float dt) {
    if (s_recv_list.empty()) return;
    // 从消息队列前面提取消息
    auto pkg = s_recv_list.front();
    s_recv_list.pop_front();
    //dispatch
    if (pkg->msg >= MESSAGE::Max_number) return;

    auto handle = s_handlers[pkg->msg];
    if(handle) {
        handle(pkg);
        delete pkg; // 释放此消息占用的内存空间
    } else {
        //
    }
}

bool Client::send(char *buf, int size) {
    if (!m_sock) return false; //连接已断开
    int send_size = m_sock->Send(buf, size);
    if(send_size <= 0) return false;
    for(size -= send_size; size > 0; size -= send_size) {
        send_size = m_sock->Send((buf += send_size), size);
        if(send_size <= 0) return false;
    }
    return true;
}

void Client::threadRecv() {
    connect_server();
    while(recv_data()) {
        char *buf = new char[m_pkg_recv.len];
        memcpy(buf, &m_pkg_recv, m_pkg_recv.len);
        s_recv_list.push_back((net_pkg *)buf);
    }
QUIT:
    cocos2d::log("recv failure");
    delete m_sock;
    m_sock = nullptr;
    m_connect = false;
}

bool Client::recv_data() {
    auto buf = (char *)&m_pkg_recv;
    int rev_size = m_sock->Recv(buf, sizeof(net_pkg), 0);
    if(rev_size <= 0) return false;

    char *p = buf;
    for(int n = m_pkg_recv.len - rev_size; n > 0; ) {
        rev_size = m_sock->Recv((p += rev_size), n);
        if(rev_size <= 0) return false;
        n -= rev_size;
    }

    return true;
}
