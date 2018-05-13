#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QColor>
#include "chess.h"

namespace Ui {
class ChessBoard;
}

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();

    void DownChess(int Row, int Column, int player);
    void backPace(int Row, int Column);
    void InitData();
    void setLastChessRed(int Row, int Column);
    void setLastPace(int Row, int Column){lastRow = Row; lastColumn = Column;}

private:
    Ui::ChessBoard *ui;
    int chessNumber;//棋子数量
    int lastRow;
    int lastColumn;
    std::unordered_map<int, std::unordered_map<int, Chess *>*> chessOnBoard;//行——列——棋子对象

    void paintEvent(QPaintEvent *event);

};

#endif // CHESSBOARD_H
