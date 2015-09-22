#include <QDebug>
#include <QThread>
#include "room.h"
#include "dialog.h"
#include "handler.h"

extern Dialog *g_dialog;

MsgHandler::MsgHandler(QTcpSocket *sock) {
    m_socket = sock;
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onStateChange(QAbstractSocket::SocketState)));

    m_handlers = new Handler[MESSAGE::Max_number]; //分配处理器Vector
    for(int i = 0; i < MESSAGE::Max_number; i++) { m_handlers[i] = nullptr; }

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
        for(auto name : Room::list()) { //枚举房间列表并发送
            pkg->arg1 = 1;
            pkg->arg2 = strlen(p);
            self->Reply(pkg, name.toStdString().c_str());
        }
        pkg->arg1 = 0; //枚举结束
        self->Reply(pkg);
    };
    HANDLER(create_room) { //创建房间
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
        g_dialog->updateRoomList();
    };
    HANDLER(join_room) { //加入某个房间
        if(self->member()->join_room(pkg->data)) {
            self->Reply(pkg, 1);
        } else {
            self->Reply(pkg, 0, self->member()->error());
        }
    };
    HANDLER(quit_room) { //退出房间
        if(self->member()->quit_room()) {
            self->Reply(pkg, 1);
        } else {
            self->Reply(pkg, 0, self->member()->error());
        }
        g_dialog->updateRoomList();
    };
    HANDLER(room_members) { //获取房间成员列表
        self->member()->room()->broad_members();
    };
    HANDLER(start_game) { //通知各客户端开始游戏
        self->member()->room()->start_game();
    };
    HANDLER(set_ready) { //设置准备状态
        auto room = self->member()->room();
        if(!room) return;
        if(pkg->arg1) {
            self->member()->set_ready_1();
            if(room->checkAllReady()) {
                //start game
            }
        } else {
            self->member()->set_ready_0();
        }
        room->broad_members();
    };
    HANDLER(set_team) { //
        auto room = self->member()->room();
        if(!room) return;
        if(room->set_team(self->member(), pkg->arg1)) {
            self->Reply(pkg, 1);
            room->broad_members();
        } else {
            self->Reply(pkg, 0, room->error());
        }
    };

    auto handler_update = [](MsgHandler *self, net_pkg *pkg) {
        self->member()->room()->broadcast(pkg, pkg->len);
    };

    m_handlers[MESSAGE::update_state] = handler_update;
    m_handlers[MESSAGE::update_speed] = handler_update;
    m_handlers[MESSAGE::update_blood] = handler_update;

    m_handlers[MESSAGE::update_position] = handler_update;
    m_handlers[MESSAGE::update_angle] = handler_update;

    m_handlers[MESSAGE::action_move] = handler_update;
    m_handlers[MESSAGE::action_stop] = handler_update;

    m_handlers[MESSAGE::do_skill] = handler_update;
    m_handlers[MESSAGE::on_attacked] = handler_update;
}

MsgHandler::~MsgHandler() {
    if(m_member) delete m_member;
    delete[] m_handlers;
}

void MsgHandler::onDisconnected() {
    qDebug() << "[Connect:] "
             << (m_member ? member()->name() : m_socket->peerName())
             << "disconnected.";
    if(m_member) m_member->quit_room();
    delete this;
}

void MsgHandler::handle() {
    auto pkg = &m_buf;

    int len = 0;
    auto rest = m_bytes_to_recv < 0 ? -m_bytes_to_recv : 0;

    // 粘包处理
    while(true) {
        rest -= len;

        // 处理包
        if(pkg->msg < MESSAGE::Max_number) {
            auto handler = m_handlers[pkg->msg];
            if(handler) handler(this, pkg); //交由子类处理器处理
        } else {
            qDebug() << "[Error message]"
                 << m_socket->peerName();
            //m_socket->disconnectFromHost();
            break;
        }

        len = pkg->len;

        if (rest > 0) {
            int len = pkg->len;
            char *buf = (char *)pkg;
            for (int i = 0; i < rest; i++) buf[i] = buf[i + len];
            pkg = (net_pkg *)(buf + len);
        } else
            break;
    }
}

//异步回调式的完整性接收处理
bool MsgHandler::Recv() {
    auto pkg = &m_buf;
    int rev_size = 0;
    int to_recv = 0;

    bool isHead = m_bytes_to_recv <= 0; //是否为接收头信息


    if(isHead) {
        to_recv = MAX_PKG_LENGTH + NET_PKG_SIZE;
        m_buf_to_recv = (char *)pkg;
    } else { // 断包情况处理
        to_recv = m_bytes_to_recv;
        m_buf_to_recv += m_bytes_to_recv;
    }

    rev_size = m_socket->read(m_buf_to_recv, to_recv);

    if(isHead) {
        Q_ASSERT(pkg->len < MAX_PKG_LENGTH);
        m_bytes_to_recv = pkg->len - rev_size;
    } else { // 断包情况处理
        m_bytes_to_recv -= rev_size;
    }

    return m_bytes_to_recv <= 0; //条件成立则数据已接收完
}

int MsgHandler::_Reply(net_pkg *p, int size) {
    p->len = size;
    int ret = m_socket->write((const char *)p, (qint64)size);
    m_socket->flush();
    return ret;
}
//数据到达时调用
void MsgHandler::onReadyRead() {
    if(endHandle()) {
        qDebug() << "[Connect:] "
                 << (m_member ? member()->name() : m_socket->peerName())
                 << "endHandle.";
        m_socket->disconnectFromHost();
    }
    if(Recv()) handle(); //数据接收完整后处理
}

void MsgHandler::onStateChange(QAbstractSocket::SocketState state) {
    switch(state) {
    case QAbstractSocket::UnconnectedState:
        break;
    case QAbstractSocket::HostLookupState:
        break;
    case QAbstractSocket::ConnectingState:
        break;
    case QAbstractSocket::ConnectedState:
        break;
    case QAbstractSocket::BoundState:
        break;
    case QAbstractSocket::ClosingState:
        break;
    case QAbstractSocket::ListeningState:
        break;
    }
}
