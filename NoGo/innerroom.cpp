#include "innerroom.h"
#include "ui_innerroom.h"
#include "player.h"
#include <QMainWindow>
#include "waitingarea.h"
#include <QDebug>
#include "playingface.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <QMessageBox>

InnerRoom::InnerRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InnerRoom)
{
    ui->setupUi(this);
}
void InnerRoom::Enter(QWidget *last, bool playerType, QString RoomID, NetworkTool *nt)
{
    playerNumber = 1;
    this->NT = nt;
    this->last = last;
    ui->RoomID->setText(RoomID);
    IamHost = playerType;
    NT->stopReceiver();
    QUdpSocket *receiver = new QUdpSocket(this);
    receiver->bind(PORTNUMBER, QUdpSocket::ShareAddress);
    connect(receiver, SIGNAL(readyRead()), this, SLOT(on_network_receive()));
    NT->setRecevier(receiver);
    me = new Player(NT->getLocalName(), NT->getLocalPercent(), playerType, ui->widget_5);
    ui->verticalLayout_3->addWidget(me);
    if(playerType)
    {
        ui->pushButton->setText(QMainWindow::tr("开始游戏"));
        ui->pushButton_2->setText(QMainWindow::tr("解散房间"));
        srbm = new SetRoomBaseMessage(this);
    }
    else
    {
        ui->pushButton->setText(QMainWindow::tr("准备"));
        ui->pushButton_2->setText(QMainWindow::tr("退出房间"));
        ui->pushButton_3->hide();
    }
    ui->BoardSize->setText(QMainWindow::tr("9X9"));
    ui->TotalTime->setText(QMainWindow::tr("900"));
    ui->RoundTime->setText(QMainWindow::tr("60"));
    this->show();
    rival = nullptr;
}
QString InnerRoom::toString()
{
    return ui->RoomID->text() + " " + QString::number(playerNumber) + " " + ui->BoardSize->text();
}
InnerRoom::~InnerRoom()
{
    delete ui;
}
//准备按钮/开始游戏按钮事件
void InnerRoom::on_pushButton_clicked()
{
    QString text = ui->pushButton->text();
    if(text == QMainWindow::tr("准备"))
    {
        ui->pushButton->setText(QMainWindow::tr("取消准备"));
        NetworkTool::sendMessage(QMainWindow::tr("07"), NT->getRivalIP());
    }
    else if(text == QMainWindow::tr("开始游戏"))
    {
        if(playerNumber != 2) return;
        if(!rival->getStatus())
        {
            QMessageBox::information(this, QMainWindow::tr("说明"), QMainWindow::tr("对方未准备"), QMessageBox::Ok);
            return;
        }
        srand(time(NULL));
        QString s = "", s2;
        myTurn = rand() % 2 + 1;
        if(myTurn == 1)
        {
            s = "o";
            s2 = "t";
        }
        else
        {
            s = "t";
            s2 = "o";
        }
        NetworkTool::sendMessage(QMainWindow::tr("09 ") + s, NT->getLocalIP());
        NetworkTool::sendMessage(QMainWindow::tr("09 ") + s2, NT->getRivalIP());
    }
    else if(text == QMainWindow::tr("取消准备"))
    {
        ui->pushButton->setText(QMainWindow::tr("准备"));
        NetworkTool::sendMessage(QMainWindow::tr("08"), NT->getRivalIP());
    }
}
//退出房间按钮/解散房间按钮事件
void InnerRoom::on_pushButton_2_clicked()
{
    QString text = ui->pushButton_2->text();
    if(text == QMainWindow::tr("退出房间"))
    {
        NetworkTool::sendMessage(QMainWindow::tr("02"), NT->getRivalIP());//向对手发送离开房间消息
        NetworkTool::sendMessage(QMainWindow::tr("03"), NT->getLocalIP());//向自己发送解散房间消息
    }
    else if(text == QMainWindow::tr("解散房间"))
        NetworkTool::sendMessage(QMainWindow::tr("03") + " " + ui->RoomID->text());//广播解散房间消息
}
void InnerRoom::on_network_receive()
{
    QHostAddress sender;
    QByteArray datagram;
    QUdpSocket *receiver = NT->getReceiver();
    datagram.resize(receiver->pendingDatagramSize());
    receiver->readDatagram(datagram.data(), datagram.size(), &sender);
    QString msg = datagram;
    QList<QString> message = msg.split(" ");
    int type = message.at(0).toInt();
    switch(type)
    {
    case 0:NetworkTool::sendMessage(QMainWindow::tr("04") + " " + this->toString());break;//接收到“获取房间信息”
    case 1://接收到“进入房间消息”
    {
        rival = new Player(message.at(1), message.at(2), false, ui->widget_5);
        ui->verticalLayout_3->addWidget(rival);
        QList<QString> str = sender.toString().split(":");
        NT->setRivalIP(str.at(str.size() - 1));
        NetworkTool::sendMessage(QMainWindow::tr("05") + " " + me->toString(), NT->getRivalIP());
    };break;
    case 2://接收到“离开房间消息”
    {
        ui->verticalLayout_3->removeWidget(rival);
        rival->hide();
        delete rival;
        rival = nullptr;
        NT->setRivalIP("");
    };break;
    case 3://接收到“解散房间消息”
    {
        ui->verticalLayout_3->removeWidget(me);
        me->hide();
        delete me;
        if(rival != nullptr)
        {
            ui->verticalLayout_3->removeWidget(rival);
            rival->hide();
            delete rival;
        }
        this->hide();
        last->show();
        NT->stopReceiver();
        WaitingArea *temp = (WaitingArea*)last;
        temp->RefreshRoomList();
    };break;
    case 5://接收到“发送玩家信息消息”
    {
        rival = new Player(message.at(1), message.at(2), true, ui->widget_5);
        ui->verticalLayout_3->addWidget(rival);
    };break;
    case 6://接收到“房间基本信息改变消息”
    {
        ui->BoardSize->setText(message.at(1));
        ui->TotalTime->setText(message.at(2));
        ui->RoundTime->setText(message.at(3));
        QList<QString> bs = message.at(1).split("X");
        GameMessage::setChessBoardSize(bs.at(0).toInt(), bs.at(1).toInt());
    };break;
    case 7:rival->changeStatus();break;//接收到“玩家准备消息”
    case 8:rival->changeStatus();break;//接收到“玩家取消准备消息”
    case 9://接收到“开始游戏消息”
    {
        NT->stopReceiver();
        QString msg = message.at(1);
        if(msg == QMainWindow::tr("o")) myTurn = 1;
        else if(msg == QMainWindow::tr("t")) myTurn = 2;
        WaitingArea *temp = (WaitingArea*)last;
        QWidget *pf = temp->getPlayingFace();
        PlayingFace *play = (PlayingFace *)pf;
        this->hide();
        play->Enter(this, 4);
    };break;
    }
}
//设置按钮
void InnerRoom::on_pushButton_3_clicked()
{
    srbm->Enter(NT);
}
void InnerRoom::getTime(int &TotalTime, int &RoundTime)
{
    TotalTime = ui->TotalTime->text().toInt();
    RoundTime = ui->RoundTime->text().toInt();
}
