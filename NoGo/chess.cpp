#include "chess.h"
#include "ui_chess.h"
#include <QPainter>

Chess::Chess(int BelongPlayer, int number, int Row, int Column, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chess)
{
    ui->setupUi(this);
    this->row = Row;
    this->column = Column;
    this->belongPlayer = BelongPlayer;
    ui->label->setText(QString::number(number));
    ui->label->setStyleSheet("color:red");
    picture = BelongPlayer == 1 ? ":/resource/BlackChess.bmp" : ":/resource/WhiteChess.bmp";
//    ui->label->setScaledContents(true);
//    ui->label->setPixmap(QPixmap(picture));
    this->show();
}
void Chess::setColor()
{
    QString temp = belongPlayer == 2 ? "color:black" : "color:white";
    ui->label->setStyleSheet(temp);
}
void Chess::setRedColor()
{
    ui->label->setStyleSheet("color:red");
}
Chess::~Chess()
{
    delete ui;
}
void Chess::paintEvent(QPaintEvent *e)
{
    int chessWidth = GameMessage::getChessWidth(), chessHeight = GameMessage::getChessHeight();
    int y = this->row * chessHeight - chessHeight / 2;
    int x = this->column * chessWidth - chessWidth / 2;
    this->setGeometry(x, y, chessWidth, chessHeight);
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), QPixmap(picture));
}
