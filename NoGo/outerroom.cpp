#include "outerroom.h"
#include "ui_outerroom.h"
#include <QMainWindow>

OuterRoom::OuterRoom(NetworkTool *NT, InnerRoom *inner, QString HostIP, QWidget *root, int roomID, int personNumber, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OuterRoom)
{
    ui->setupUi(this);
    this->innerRoom = inner;
    ui->label->setText(QString::number(roomID));
    ui->label_2->setText(QString::number(personNumber));
    this->NT = NT;
    this->root = root;
    this->HostIP = HostIP;
}
void OuterRoom::setRoomID(int roomID)
{
    ui->label->setText(QString::number(roomID));
}
void OuterRoom::setPerson(int number)
{
    ui->label_2->setText(QString::number(number));
}
void OuterRoom::setBoardSize(QString size)
{
    ui->label_3->setText(size);
}
void OuterRoom::setBaseMessage(QString roomID, QString number, QString size)
{
    ui->label->setText(number);
    ui->label_2->setText(roomID);
    ui->label_3->setText(size);
}
int OuterRoom::getPersonNumber()
{
    return ui->label_2->text().toInt();
}
OuterRoom::~OuterRoom()
{
    delete ui;
}
//进入房间按钮事件
void OuterRoom::on_pushButton_clicked()
{
    if(ui->label_2->text() == QMainWindow::tr("2")) return;
    NT->setRivalIP(this->HostIP);
    innerRoom->Enter(root, false, ui->label->text(), NT);
    root->hide();
}
