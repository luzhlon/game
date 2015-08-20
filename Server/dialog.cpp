#include "dialog.h"
#include "ui_dialog.h"
#include "room.h"

extern void startWork();

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    startWork();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateRoomList() {
    //
}

void Dialog::on_button_create_clicked()
{

}

void Dialog::on_check_switch_toggled(bool checked)
{
    if(checked) {
        startWork();
        return;
    }
}
