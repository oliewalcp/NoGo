#include "chessboard.h"
#include "ui_chessboard.h"

ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessBoard)
{
    ui->setupUi(this);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(195, 195, 195));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}
void ChessBoard::InitData()
{
    chessNumber = lastRow = lastColumn = 0;
    int row = GameMessage::getRow(), column = GameMessage::getColumn();
    for(int i = 1, j; i <= row; i++)
    {
        std::unordered_map<int, Chess*> *temp;
        auto it = chessOnBoard.find(i);
        if(it == chessOnBoard.end())
        {
            temp = new std::unordered_map<int, Chess*>;
            chessOnBoard[i] = temp;
        }
        else temp = it->second;
        for(j = 1; j <= column; j++)
        {
            if(temp->find(j) != temp->end() && (*temp)[j] != nullptr)
            {
                Chess *c = (*temp)[j];
                c->hide();
                delete c;
            }
            (*temp)[j] = nullptr;
        }
    }
}
ChessBoard::~ChessBoard()
{
    delete ui;
}
void ChessBoard::DownChess(int Row, int Column, int player)
{
    if(lastRow != 0 && lastColumn != 0)
    {
        Chess *c = (*(chessOnBoard[lastRow]))[lastColumn];
        c->setColor();//还原颜色
    }
    (*(chessOnBoard[Row]))[Column] = new Chess(player, ++chessNumber, Row, Column, this);
    setLastPace(Row, Column);
}
void ChessBoard::backPace(int Row, int Column)
{
    Chess *chess = (*(chessOnBoard[Row]))[Column];
    chess->hide();
    delete chess;
    (*(chessOnBoard[Row]))[Column] = nullptr;
    chessNumber--;
}
void ChessBoard::setLastChessRed(int Row, int Column)
{
    Chess *c = (*(chessOnBoard[Row]))[Column];
    c->setRedColor();
}
//重绘事件
void ChessBoard::paintEvent(QPaintEvent *event)
{
    int chessWidth, chessHeight;
    int boardRow = GameMessage::getRow(), boardColumn = GameMessage::getColumn();
    QSize rect = this->size();//获取棋盘的大小
    chessWidth = rect.width() / (boardColumn + 1);
    chessHeight = rect.height() / (boardRow + 1);
    GameMessage::setChessSize(chessWidth, chessHeight);

    QPainter *paintTool = new QPainter(this);
    QPen *pen = new QPen(Qt::black, 2);//创建画笔
    paintTool->setPen(*pen);

    for(int i = 1, j; i <= boardRow; i++)
    {
        for(j = 1; j <= boardColumn; j++)
        {
            unsigned int j_chessWidth = j * chessWidth, i_chessHeight = i * chessHeight;
            paintTool->drawLine(j_chessWidth, chessHeight, j_chessWidth, boardRow * chessHeight);//画竖线
            paintTool->drawLine(chessWidth, i_chessHeight, boardColumn * chessWidth, i_chessHeight);//画横线
        }
    }

    delete pen;
    delete paintTool;
}
