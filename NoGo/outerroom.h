#ifndef OUTERROOM_H
#define OUTERROOM_H

#include <QWidget>
#include "gamemessage.h"
#include "innerroom.h"

namespace Ui {
class OuterRoom;
}

class OuterRoom : public QWidget
{
    Q_OBJECT

public:
    explicit OuterRoom(NetworkTool *NT, InnerRoom *inner, QString HostIP, QWidget *root, int roomID, int personNumber = 1, QWidget *parent = 0);
    ~OuterRoom();

    void setPerson(int number);
    void setRoomID(int roomID);
    void setBoardSize(QString size);
    void setBaseMessage(QString roomID, QString number, QString size);
    int getPersonNumber();

private slots:
    void on_pushButton_clicked();

private:
    Ui::OuterRoom *ui;
    NetworkTool *NT;
    QWidget *root;
    QString HostIP;
    InnerRoom *innerRoom;
};

#endif // OUTERROOM_H
