#include <QDebug>
#include "room.h"
#include "handler.h"

MsgHandler::MsgHandler() {
    m_handlers = new Handler[MESSAGE::Max_number]; //分配处理器Vector

    HANDLER(authentication) { //
        auto member = new Member(pkg->data, self);
        if(member->error()) {
            self->Reply(pkg, 0, member->error());
            delete member;
            self->endHandle();
            qDebug() << "Member:" << member->name() << "authentication failure.";
            return;
        } else {
            member->m_role_id = pkg->arg1;
            self->Reply(pkg, 1);
            qDebug() << "New member:" << member->name();
        }
    };
    HANDLER(room_list) { //房间列表
        auto p = pkg->data;
        for(auto name : Room::list()) {
            strcpy(p, name.toStdString().c_str());
            p += name.length() + 1;
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
            qDebug() << "Room:" << room->name() << " created by ";
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
    delete m_member;
    delete m_socket;
    delete m_handlers;
}

/*
bool MsgHandler::authenticate() {
    if(Recv() <=  0) {
        return false;
    }
    if(m_buf.arg1 != 0 || m_buf.arg2 !=0) {
        ReplyString(&m_buf, "error format");
        return false;
    }
    m_member = new Member(nick_name, this);
    m_buf.arg1 = 1;
    Reply1(&m_buf);
    return true;
} // */

void MsgHandler::loopHandle() {
    while(true) {
        int len = Recv();
        if(len <= 0) {
            qDebug() << "Connect " << member()->m_name
                     << "quited:" << m_socket->errorString();
            break;
        }
        handle();
        if(endHandle()) {
            qDebug() << "Connect " << member()->m_name << "quited.";
            break;
        }
    }
}

void MsgHandler::handle(net_pkg *p) {
    p = p ? p : &m_buf;
    if(p->msg < MESSAGE::Max_number) {
        m_handlers[p->msg](this, p); //交由子类处理器处理
    } else {
        qDebug() << member()->m_name <<":Error message";
    }
}

int MsgHandler::Recv(char *buf) {
    buf = buf ? buf : (char *)&m_buf;
    auto pkg = (net_pkg *)buf;
    int rev_size = 1;
    //阻塞直到可读
    while(!m_socket->waitForReadyRead()); 
    rev_size = m_socket->read(buf, MAX_PKG_LENGTH + NET_PKG_SIZE);
    if(rev_size <= 0) { return rev_size; }
    Q_ASSERT(pkg->len < MAX_PKG_LENGTH);
    //循环接收保证获取数据完整
    for(int n = pkg->len - rev_size; n > 0; ) {
        while(!m_socket->waitForReadyRead()); //阻塞直到可读
        rev_size = m_socket->read((buf += rev_size), n);
        if(rev_size <= 0) { return rev_size; }
        n -= rev_size;
    }
}

int MsgHandler::_Reply(net_pkg *p, int size) {
    p->len = size;
    int ret = m_socket->write((const char *)p, (qint64)size);
    m_socket->flush();
    return ret;
}
