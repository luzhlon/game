#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <QDialog>

class QListWidgetItem;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void updateRoomList();

private slots:
    void on_button_create_clicked();

    void on_check_switch_toggled(bool checked);

    void on_list_room_itemClicked(QListWidgetItem *item);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
