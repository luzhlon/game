#include "Client.h"

Client *Client::s_client = nullptr;
char *Client::s_serverIP = SERVER_IP;
int Client::s_serverPort = 4321;
net_pkg Client::s_pkg;
Client::handler Client::s_handlers[MESSAGE::Max_number];

Client::Client() {
    ODSocket::Init();
    m_sock.Create(AF_INET, SOCK_STREAM, 0);
}

Client* Client::getInstance() {
    if(s_client) return s_client;
    s_client = new Client();
    return s_client;
}

bool Client::connectSever() {
    for(int i = 0; i < 5; i++) {
        if(m_sock.Connect(s_serverIP, s_serverPort)) {
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
                schedule_selector(Client::dispatchMsg), this, 0.05f, false);
}

void Client::dispatchMsg(float dt) {
    if(!m_pkg) return;
    //dispatch
    auto h = s_handlers[m_pkg->msg];
    if(h) {
        h(m_pkg);
    } else {
        //
    }
    //
    m_pkg = nullptr;
    m_handled = true;
}

bool Client::send(char *buf, int size) {
    int send_size = m_sock.Send(buf, size);
    if(send_size <= 0) return false;
    for(size -= send_size; size > 0; size -= send_size) {
        send_size = m_sock.Send((buf += send_size), size);
        if(send_size <= 0) return false;
    }
    return true;
}

void Client::threadRecv() {
    net_pkg pkg;
    connectSever();
    auto buf = (char *)&pkg;
    while(true) {
        while(!m_handled) sleep(1); //wait dispatch handle
        int rev_size = m_sock.Recv(buf, sizeof(net_pkg), 0);
        if(rev_size <= 0) goto QUIT;

        char *p = buf;
        for(int n = pkg.len - rev_size; n > 0; ) {
            rev_size = m_sock.Recv((p += rev_size), n);
            if(rev_size <= 0) goto QUIT;
            n -= rev_size;
        }
        //
        m_pkg = &pkg;
        m_handled = false;
    }
QUIT:
    cocos2d::log("recv failure");
}
