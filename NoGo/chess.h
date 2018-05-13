#ifndef CHESS_H
#define CHESS_H

#include <QWidget>
#include "gamemessage.h"

namespace Ui {
class Chess;
}

class Chess : public QWidget
{
    Q_OBJECT

public:
    explicit Chess(int BelongPlayer, int number, int Row, int Column, QWidget *parent = 0);
    ~Chess();

    int getBelongPlayer(){return belongPlayer;}
    void setColor();
    void setRedColor();

private:
    Ui::Chess *ui;

    int belongPlayer;//所属玩家
    int row;
    int column;
    QString picture;

    void paintEvent(QPaintEvent *e);
};

#endif // CHESS_H
