#include "waitingarea.h"
#include "ui_waitingarea.h"
#include "mainmenuface.h"
#include <QScrollArea>
#include <QMainWindow>
#include <QDebug>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <QFile>
#include <QSettings>

WaitingArea::WaitingArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingArea)
{
    ui->setupUi(this);
    QScrollArea scroll;
    scroll.setWidget(ui->RoomListArea);
    NT = new NetworkTool;
}
bool WaitingArea::Enter(QWidget *last, InnerRoom *inner)
{
    QString LocalIP = NetworkTool::getLocalMachineIP();
    if(LocalIP == "") return false;
    this->innerRoom = inner;
    NT->setLocalIP(LocalIP);
    NT->setRivalIP("");
    this->lastPanel = last;
    RoomList.clear();
    receiver = new QUdpSocket(this);
    if(!receiver->bind(PORTNUMBER, QUdpSocket::ShareAddress))
    {
        delete receiver;
        return false;
    }
    this->readDataFile();
    connect(receiver, SIGNAL(readyRead()), this, SLOT(on_udpSocket_receive()));
    NT->setRecevier(receiver);
    on_pushButton_clicked();
    this->show();
    return true;
}
WaitingArea::~WaitingArea()
{
    delete ui;
}
//返回主菜单按钮事件
void WaitingArea::on_pushButton_2_clicked()
{
    receiver->close();
    delete receiver;
    this->hide();
    this->lastPanel->show();
}
void WaitingArea::RefreshRoomList()
{
    receiver = new QUdpSocket(this);
    receiver->bind(PORTNUMBER, QUdpSocket::ShareAddress);
    connect(receiver, SIGNAL(readyRead()), this, SLOT(on_udpSocket_receive()));
    NT->setRecevier(receiver);
    on_pushButton_clicked();
}
//刷新按钮事件
void WaitingArea::on_pushButton_clicked()
{
    for(auto it = RoomList.begin(); it != RoomList.end(); it++)
    {
        OuterRoom *outR = it->second;
        ui->verticalLayout_5->removeWidget(outR);
        outR->hide();
        delete outR;
    }
    RoomList.clear();
    NetworkTool::sendMessage(QMainWindow::tr("00"));
}
//网络消息接收事件
void WaitingArea::on_udpSocket_receive()
{
    QHostAddress sender;
    QByteArray datagram;
    datagram.resize(receiver->pendingDatagramSize());
    receiver->readDatagram(datagram.data(), datagram.size(), &sender);
    QList<QString> str = sender.toString().split(":");
    QString localip = str.at(str.size() - 1);
    if(localip == NT->getLocalIP()) return;
    QString msg = datagram;
    QList<QString> message = msg.split(" ");
    QString type = message.at(0);
    if(type == "04")//获取房间基本信息
    {
        int roomid = message.at(1).toInt();
        if(RoomList.find(roomid) != RoomList.end())
        {
            OuterRoom *temp = RoomList[roomid];
            temp->setBaseMessage(message.at(1), message.at(2), message.at(3));
        }
        else
        {
            OuterRoom *temp = new OuterRoom(NT, innerRoom, sender.toString(), this, message.at(1).toInt(), 1, ui->RoomListArea);
            RoomList[message.at(1).toInt()] = temp;
            ui->verticalLayout_5->addWidget(temp);
        }
    }
    else if(type == "03")//解散房间
    {
        int roomid = message.at(1).toInt();
        if(RoomList.find(roomid) == RoomList.end()) return;
        OuterRoom *temp = RoomList[roomid];
        ui->verticalLayout_5->removeWidget(temp);
        temp->hide();
        delete temp;
    }
}
//创建房间按钮事件
void WaitingArea::on_pushButton_3_clicked()
{
    this->hide();
    NT->setPersonalMessage(ui->NameInput->text(), ui->WinPercent->text());
    innerRoom->Enter(this, true, QString::number(this->getRoomID()), this->NT);
}
int WaitingArea::getRoomID()
{
    srand(time(NULL));
    int roomid;
    unsigned int max = 1000, i = 0;
    while(true)
    {
        roomid = rand() % (max - 1) + 1;
        if(RoomList.find(roomid) == RoomList.end())
            return roomid;
        if(++i == max - 1) max *= 100;
    }
}

void WaitingArea::on_NameInput_editingFinished()
{
    NT->setPersonalMessage(ui->NameInput->text(), ui->WinPercent->text());
}
QWidget* WaitingArea::getPlayingFace()
{
    MainMenuFace *temp = (MainMenuFace *)lastPanel;
    return temp->getPlayingFace();
}
void WaitingArea::readDataFile()
{
    QSettings *setfile = new QSettings("data.ini", QSettings::IniFormat);
    QString str = setfile->value("un").toString();;
    if(str != "") ui->NameInput->setText(str);
    else ui->NameInput->setText(QMainWindow::tr(""));
    str = setfile->value("wn").toString();
    if(str != "") ui->Win->setText(str);
    else ui->Win->setText(QMainWindow::tr("0"));
    str = setfile->value("le").toString();;
    if(str != "") ui->Lose->setText(str);
    else ui->Lose->setText(QMainWindow::tr("0"));
    str = setfile->value("pc").toString();
    if(str != "") ui->WinPercent->setText(str);
    else ui->WinPercent->setText(QMainWindow::tr("0"));
    delete setfile;
}
void WaitingArea::writeDataFile()
{
    QFile::remove("data.ini");
    QSettings *setfile = new QSettings("data.ini", QSettings::IniFormat);
    setfile->setValue("un", ui->NameInput->text());
    setfile->setValue("wn", ui->Win->text());
    setfile->setValue("le", ui->Lose->text());
    setfile->setValue("pc", ui->WinPercent->text());
    delete setfile;
}
