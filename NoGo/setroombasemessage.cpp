#include "setroombasemessage.h"
#include "ui_setroombasemessage.h"
#include <QMainWindow>

SetRoomBaseMessage::SetRoomBaseMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetRoomBaseMessage)
{
    ui->setupUi(this);
    QValidator *val = new QIntValidator(2, 200000000, this);//设定输入框的输入范围
    ui->lineEdit->setValidator(val);
    ui->lineEdit_2->setValidator(val);
    ui->lineEdit_3->setValidator(val);
    ui->lineEdit_4->setValidator(val);
}
void SetRoomBaseMessage::Enter(NetworkTool *nt)
{
    this->NT = nt;
    this->show();
}
SetRoomBaseMessage::~SetRoomBaseMessage()
{
    delete ui;
}
//确定按钮
void SetRoomBaseMessage::on_pushButton_clicked()
{
    QString base = QMainWindow::tr("06") + " " + ui->lineEdit->text() + "X" + ui->lineEdit_2->text() +
            " " + ui->lineEdit_3->text() + " " + ui->lineEdit_4->text();
    NetworkTool::sendMessage(base, NT->getLocalIP());
    NetworkTool::sendMessage(base, NT->getRivalIP());
    this->hide();
}
//取消按钮
void SetRoomBaseMessage::on_pushButton_2_clicked()
{
    this->hide();
}
