#ifndef PLAYINGFACE_H
#define PLAYINGFACE_H

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QMouseEvent>
#include <QThread>
#include "chessboard.h"
#include "gamemessage.h"

#define GetDisplayTime GetTime

class Thread : public QThread
{
    Q_OBJECT
private:
    int row, column;
    int myTurn;
protected:
    void run();
public:
    Thread(int myTurn, QObject *parent = 0)
    {
        row = column = 0;
        this->myTurn = myTurn;
    }
    ~Thread(){}
    void setDownChess(int row, int column)
    {
        this->row = row;
        this->column = column;
    }
signals:
    void ai_finish(int Row, int Column);
};

namespace Ui {
class PlayingFace;
}

class PlayingFace : public QWidget
{
    Q_OBJECT

public:
    explicit PlayingFace(QWidget *parent = 0);
    ~PlayingFace();

    void Enter(QWidget *last, int type);//进入游戏对弈界面

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void mousePressEvent(QMouseEvent *e);

    void on_pushButton_clicked();

    void on_udpSocket_receive();

    void on_ai_finish(int Row, int Column);

private:
    Ui::PlayingFace *ui;
    Thread *AI_Brain;//AI线程
    QWidget *last;//上一个界面
    QWidget *MainMenu;//主菜单
    ChessBoard *chess_board;//棋盘
    int type;//游戏模式类型
    int myTurn;//标记本机的玩家编号
    GameMessage *GM;
    QTimer *player1Timer;//玩家1的计时器
    QTimer *player2Timer;//玩家2的计时器
    NetworkTool *NT;
    bool InGaming;//标记是否游戏进行中
    unsigned int pauseTimeLength;//暂停时长

    unsigned int player1TotalTime;//玩家1剩余局时
    unsigned int player1RoundTime;//玩家1剩余步时
    unsigned int player2TotalTime;//玩家2剩余局时
    unsigned int player2RoundTime;//玩家2剩余步时

    void InitData();//初始化数据
    void BackPace();//悔棋操作
    void PauseGame();//暂停游戏
    void ContinueGame();//继续游戏
    void SetSeconds(int player);//设置显示的时间文本
    void TimerEvent();//计时器事件
    void ShowWinner(int winner);//宣布游戏结束
    void ExchangeTimer();//交换计时器
    void RunAI();
    QString GetTime(unsigned int time);//获取时间字符串
};

#endif // PLAYINGFACE_H
