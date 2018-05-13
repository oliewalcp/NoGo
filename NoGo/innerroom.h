#ifndef INNERROOM_H
#define INNERROOM_H

#include <QWidget>
#include "setroombasemessage.h"
#include "gamemessage.h"
#include "player.h"

namespace Ui {
class InnerRoom;
}

class InnerRoom : public QWidget
{
    Q_OBJECT

public:
    explicit InnerRoom(QWidget *parent = 0);
    ~InnerRoom();

    void Enter(QWidget *last, bool playerType, QString RoomID, NetworkTool *nt);
    void getTime(int &TotalTime, int &RoundTime);
    NetworkTool *getNetworkTool(){return NT;}
    int getMyTurn(){return myTurn;}

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_network_receive();

    void on_pushButton_3_clicked();

private:
    Ui::InnerRoom *ui;
    SetRoomBaseMessage *srbm;
    QWidget *last;
    bool IamHost;
    NetworkTool *NT;
    int playerNumber;//房间人数
    Player *me;
    Player *rival;
    int myTurn;

    QString toString();
};

#endif // INNERROOM_H
