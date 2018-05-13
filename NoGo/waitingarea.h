#ifndef WAITINGAREA_H
#define WAITINGAREA_H

#include <QWidget>
#include "outerroom.h"
#include "gamemessage.h"

namespace Ui {
class WaitingArea;
}

class WaitingArea : public QWidget
{
    Q_OBJECT

public:
    explicit WaitingArea(QWidget *parent = 0);
    ~WaitingArea();

    bool Enter(QWidget *last, InnerRoom *inner);
    void RefreshRoomList();
    QWidget *getPlayingFace();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_udpSocket_receive();

    void on_pushButton_3_clicked();

    void on_NameInput_editingFinished();

private:
    Ui::WaitingArea *ui;
    QWidget *lastPanel;
    QUdpSocket *receiver;
    NetworkTool *NT;
    InnerRoom *innerRoom;
    std::unordered_map<int, OuterRoom*> RoomList;//房间列表（房间号——房间条）

    int getRoomID();//获取不重复的房间号
    void readDataFile();//读文件
    void writeDataFile();//写文件
};

#endif // WAITINGAREA_H
