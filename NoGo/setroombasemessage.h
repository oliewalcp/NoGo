#ifndef SETROOMBASEMESSAGE_H
#define SETROOMBASEMESSAGE_H

#include <QDialog>
#include "gamemessage.h"

namespace Ui {
class SetRoomBaseMessage;
}

class SetRoomBaseMessage : public QDialog
{
    Q_OBJECT

public:
    explicit SetRoomBaseMessage(QWidget *parent = 0);
    ~SetRoomBaseMessage();

    void Enter(NetworkTool *nt);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SetRoomBaseMessage *ui;
    NetworkTool *NT;

};

#endif // SETROOMBASEMESSAGE_H
