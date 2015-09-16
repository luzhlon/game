﻿#include "dialog.h"
#include "ui_dialog.h"
#include "room.h"

extern void startWork();

Dialog *g_dialog;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    Qt::WindowFlags flags=Qt::Dialog;

    flags |= Qt::WindowMinimizeButtonHint; //最小化按钮
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowModal;
    setWindowFlags(flags);

    ui->setupUi(this);
    startWork();
    g_dialog = this;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateRoomList() {
    //Clear all
    for(QListWidgetItem *item; item = ui->list_room->takeItem(0);)
        delete item;
    //Add
    auto list = Room::list();
    for(QString name : list) {
        ui->list_room->addItem(name);
    }
}

void Dialog::on_button_create_clicked() {

}

void Dialog::on_check_switch_toggled(bool checked) {
    if(checked) {
        startWork();
        return;
    }
}

void Dialog::on_list_room_itemClicked(QListWidgetItem *item)
{
    for(QListWidgetItem *itm; itm = ui->list_member->takeItem(0);)
        delete itm;

    auto name = item->text();
    auto room = Room::g_room[name];
    if(!room) return;
    for(auto mem : room->m_members) {
        ui->list_member->addItem(mem->name());
    }
}
