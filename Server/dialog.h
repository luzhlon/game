#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <QDialog>

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
    void on_btnTurnOn_clicked();

    void on_button_create_clicked();

    void on_check_switch_toggled(bool checked);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
