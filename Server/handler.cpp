#include <QDebug>
#include <QThread>
#include "room.h"
#include "handler.h"

MsgHandler::MsgHandler(QTcpSocket *sock) {
    m_socket = sock;
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    m_handlers = new Handler[MESSAGE::Max_number]; //分配处理器Vector

    HANDLER(authentication) { //
        if(self->setMember(pkg->data, pkg->arg1)) {
            self->Reply(pkg, 1);
            qDebug() << "[New member]:" << self->member()->name();
        } else {
            self->Reply(pkg, 0, self->error());
            self->endHandle();
            qDebug() << "<Member>:"
                     << self->socket()->peerName()
                     << "authentication failure.";
        }
    };
    HANDLER(room_list) { //房间列表
        auto p = pkg->data;
        pkg->arg1 = 0;
        for(auto name : Room::list()) {
            strcpy(p, name.toStdString().c_str());
            p += name.length() + 1;
            pkg->arg1 ++;
        }
        self->Reply(pkg, pkg->data, p - pkg->data);
    };
    HANDLER(create_room) {
        auto room = new Room(pkg->data);
        if(room->error()) {
            self->Reply(pkg, 0, room->error());
            delete room;
        } else {
            self->Reply(pkg, 1);
            qDebug() << "[Room]:"
                     << room->name()
                     << " created by "
                     << self->member()->name();
        }
    };
    HANDLER(join_room) {
        if(self->member()->join_room(pkg->data)) {
            self->Reply(pkg, 1);
        } else {
            self->Reply(pkg, 0, self->member()->error());
        }
    };
    HANDLER(quit_room) {
        if(self->member()->quit_room()) {
            self->Reply(pkg, 1);
        } else {
            self->Reply(pkg, 0, self->member()->error());
        }
    };
    HANDLER(room_members) {
        self->member()->room()->broadMembers();
    };
    HANDLER(start_game) {
        self->member()->room()->broadcast(pkg);
    };
    HANDLER(set_ready) {
        auto room = self->member()->room();
        if(!room) return;
        if(pkg->arg1) {
            self->member()->set_ready_1();
            room->broadMembers();
            room->checkAllReady();
        } else {
            self->member()->set_ready_0();
        }
    };
    HANDLER(set_team) {
        auto room = self->member()->room();
        if(!room) return;
        if(room->setTeam(self->member(), pkg->arg1)) {
            self->Reply(pkg, 1);
            room->broadMembers();
        } else {
            self->Reply(pkg, 0, room->error());
        }
    };
}

MsgHandler::~MsgHandler() {
    if(m_member) delete m_member;
    delete m_socket;
    delete m_handlers;
}

void MsgHandler::loopHandle() {
    while(true) {
        if(endHandle()) {
            qDebug() << "[Connect:] "
                     << (m_member ? member()->name() : m_socket->peerName())
                     << "endHandle.";
            break;
        }
        if(Recv()) {
            handle();
        }
    }
}

void MsgHandler::onDisconnected() {
    qDebug() << "[Connect:] "
             << (m_member ? member()->name() : m_socket->peerName())
             << "disconnected.";
    endHandle(true);
    //delete this;
    //QThread::currentThread()->exit();
}

void MsgHandler::handle(net_pkg *p) {
    p = p ? p : &m_buf;
    if(p->msg < MESSAGE::Max_number) {
        m_handlers[p->msg](this, p); //交由子类处理器处理
    } else {
        qDebug() << "[Error message]"
                 << m_socket->peerName();
    }
}

bool MsgHandler::Recv(char *buf) {
    buf = buf ? buf : (char *)&m_buf;
    auto pkg = (net_pkg *)buf;
    int rev_size = 1;

    if(!m_socket->waitForReadyRead()) return false;
    rev_size = m_socket->read(buf, MAX_PKG_LENGTH + NET_PKG_SIZE);

    if(rev_size <= 0) { return false; }
    Q_ASSERT(pkg->len < MAX_PKG_LENGTH);
    //循环接收保证获取数据完整
    for(int n = pkg->len - rev_size; n > 0; ) {

        if(!m_socket->waitForReadyRead()) return false;
        rev_size = m_socket->read((buf += rev_size), n);

        if(rev_size <= 0) { return false; }
        n -= rev_size;
    }
    return true;
}

int MsgHandler::_Reply(net_pkg *p, int size) {
    p->len = size;
    int ret = m_socket->write((const char *)p, (qint64)size);
    m_socket->flush();
    return ret;
}
