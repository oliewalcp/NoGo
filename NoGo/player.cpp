#include "player.h"
#include "ui_player.h"
#include <QMainWindow>

Player::Player(QString name, QString percent, bool host, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    ui->label->setText(name);
    ui->label_2->setText(percent);
    if(host) ui->label_3->setText(QMainWindow::tr("房主"));
    else ui->label_3->setText(QMainWindow::tr("未准备"));
}
void Player::changeStatus()
{
    QString status = ui->label_3->text();
    if(status == QMainWindow::tr("未准备"))
        ui->label_3->setText(QMainWindow::tr("已准备"));
    else if(status == QMainWindow::tr("已准备"))
        ui->label_3->setText(QMainWindow::tr("未准备"));
}
QString Player::toString()
{
    return ui->label->text() + " " + ui->label_2->text();
}
bool Player::getStatus()
{
    if(ui->label_3->text() == QMainWindow::tr("已准备")) return true;
    else return false;
}
Player::~Player()
{
    delete ui;
}
