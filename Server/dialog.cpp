#include "dialog.h"
#include "ui_dialog.h"
#include "room.h"

extern void startWork();

Dialog *g_dialog;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    startWork();
    g_dialog = this;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateRoomList() {
    int count = ui->list_room->count();
    for(int i = 0; i < count; i++) {
        ui->list_room->removeItemWidget(ui->list_room->item(i));
    }
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
