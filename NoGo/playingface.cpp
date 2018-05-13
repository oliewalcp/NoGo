#include "playingface.h"
#include "ui_playingface.h"
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QTime>
#include <thread>
#include <time.h>
#include "innerroom.h"

PlayingFace::PlayingFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayingFace)
{
    ui->setupUi(this);
    this->MainMenu = parent;
    chess_board = new ChessBoard(ui->widget);
    ui->verticalLayout->addWidget(chess_board);
    player1Timer = new QTimer(this);
    player2Timer = new QTimer(this);
    connect(player1Timer, &QTimer::timeout, this, &this->TimerEvent);
    connect(player2Timer, &QTimer::timeout, this, &this->TimerEvent);
    GM = new GameMessage;
}
void PlayingFace::TimerEvent()
{
    if(GM->getNowPlayer() == 1)
    {
        player1TotalTime--;
        player1RoundTime--;
        player1Timer->setInterval(1000);
        SetSeconds(1);
    }
    else
    {
        player2TotalTime--;
        player2RoundTime--;
        player2Timer->setInterval(1000);
        SetSeconds(2);
    }
    if(player1TotalTime <= 0 || player2TotalTime <= 0 || player1RoundTime <= 0 || player2RoundTime <= 0)
    {
        if(type == 4)
        {
            NetworkTool::sendMessage(QMainWindow::tr("17"), NT->getLocalIP());
            NetworkTool::sendMessage(QMainWindow::tr("17"), NT->getRivalIP());
        }
        else
        {
            GM->exchangePlayer();
            this->ShowWinner(GM->getNowPlayer());
        }
    }
    pauseTimeLength = clock();
}
/* 进入游戏对弈界面
 * param[last]:上一个界面
 * param[type]:类型码，0——双人，1——玩家1是AI，2——玩家2是AI，3——双方都是AI，4——玩家联机，5——AI联机
 */
void PlayingFace::Enter(QWidget *last, int type)
{
    this->last = last;
    this->type = type;
    this->InitData();
    if(type == 4 || type == 5)
    {
        ui->pushButton_3->setText(QMainWindow::tr("认输"));
        InnerRoom *temp = (InnerRoom *)last;
        int tt, rt;
        temp->getTime(tt, rt);
        this->myTurn = temp->getMyTurn();
        if(myTurn == 1) ui->label->setStyleSheet("color:red");
        else ui->label_2->setStyleSheet("color:red");
        this->NT = temp->getNetworkTool();
        ui->pushButton->setText(QMainWindow::tr("暂停游戏"));
        InGaming = true;
        QUdpSocket *receiver = new QUdpSocket(this);
        NT->setRecevier(receiver);
        receiver->bind(PORTNUMBER, QUdpSocket::ShareAddress);
        connect(receiver, SIGNAL(readyRead()), this, SLOT(on_udpSocket_receive()));
        pauseTimeLength = clock();
        player1Timer->setInterval(1000);
        player1Timer->start();
    }
    else ui->pushButton->setText(QMainWindow::tr("开始游戏"));
    if(type == 3 || type == 5) ui->pushButton->setText(QMainWindow::tr("游戏正在进行中"));
    if(type == 1 || type == 2)
    {
        this->myTurn = 3 - type;
        AI_Brain = new Thread(myTurn);
        connect(AI_Brain, SIGNAL(ai_finish(int,int)), this, SLOT(on_ai_finish(int,int)));
    }
    this->show();
}
void PlayingFace::RunAI()
{
    AI_Brain->start();
}
void PlayingFace::InitData()
{
    if(type == 4 || type == 5)
    {
        InnerRoom *temp = (InnerRoom *)last;
        int tt, rt;
        temp->getTime(tt, rt);
        this->player1TotalTime = this->player2TotalTime = tt;
        this->player1RoundTime = this->player2RoundTime = rt;
        InGaming = true;
    }
    else
    {
        this->player1TotalTime = this->player2TotalTime = GameMessage::getTotalTime();
        this->player1RoundTime = this->player2RoundTime = GameMessage::getRoundTime();
        InGaming = false;
    }
    if(type == 1 || type == 2) GameMessage::AI.init_data(GameMessage::getRow(), GameMessage::getColumn(), type);
    this->SetSeconds(1);
    this->SetSeconds(2);
    GM->Init();
    chess_board->InitData();
}
void PlayingFace::SetSeconds(int player)
{
    if(player == 1)
    {
        ui->Player1TotalTime->setText(GetDisplayTime(this->player1TotalTime));
        ui->Player1RoundTime->setText(GetDisplayTime(this->player1RoundTime));
    }
    else
    {
        ui->Player2TotalTime->setText(GetDisplayTime(this->player2TotalTime));
        ui->Player2RoundTime->setText(GetDisplayTime(this->player2RoundTime));
    }
}
QString PlayingFace::GetTime(unsigned int time)
{
    QString result = "";
    unsigned int hour = time / 3600;
    time -= hour * 3600;
    unsigned int minute = time / 60;
    unsigned int seconds = time % 60;
    if(hour > 0)
    {
        if(hour < 10) result += "0";
        result += QString::number(hour) + ":";
    }
    if(minute < 10) result += "0";
    result += QString::number(minute) + ":";
    if(seconds < 10) result += "0";
    result += QString::number(seconds);
    return result;
}
PlayingFace::~PlayingFace()
{
    delete ui;
}
//返回主菜单/认输按钮事件
void PlayingFace::on_pushButton_3_clicked()
{
    QString text = ui->pushButton_3->text();
    if(text == QMainWindow::tr("返回主菜单"))
    {
        player1Timer->stop();
        player2Timer->stop();
        this->hide();
        this->last->show();
    }
    else if(text == QMainWindow::tr("认输"))
    {
        NetworkTool::sendMessage(QMainWindow::tr("12") + " " + myTurn, NT->getLocalIP());
        NetworkTool::sendMessage(QMainWindow::tr("12") + " " + myTurn, NT->getRivalIP());
    }
}
//悔棋按钮事件
void PlayingFace::on_pushButton_2_clicked()
{
    if(!InGaming) return;
    this->BackPace();
}
//棋盘的点击事件
void PlayingFace::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton || !InGaming) return;
    if((type == 4 || type == 2 || type == 1) && myTurn != GM->getNowPlayer()) return;
    unsigned int x = e->x(), y = e->y();
    if(x > chess_board->width() || y > chess_board->height()) return;
    unsigned int chessWidth = GameMessage::getChessWidth(), chessHeight = GameMessage::getChessHeight();
    int downRow = (y + chessHeight / 2) / chessHeight, downColumn = (x + chessWidth / 2) / chessWidth;
    if(downRow > GameMessage::getRow() || downColumn > GameMessage::getColumn() || downRow < 1 || downColumn < 1) return;
    int NowPlayer = GM->getNowPlayer();
    if(!GM->downChess(downRow, downColumn, NowPlayer)) return;
    if(type == 4)
    {
        NetworkTool::sendMessage(QMainWindow::tr("10") + " " + downRow + " " + downColumn, NT->getLocalIP());
        NetworkTool::sendMessage(QMainWindow::tr("10") + " " + downRow + " " + downColumn, NT->getRivalIP());
        return;
    }
    chess_board->DownChess(downRow, downColumn, NowPlayer);
    GM->exchangePlayer();//交换棋手
    if(GM->deadChess(downRow, downColumn))
    {
        this->ShowWinner(GM->getNowPlayer());
        return;
    }
    if(type == GM->getNowPlayer())
    {
        AI_Brain->setDownChess(downRow, downColumn);
        this->RunAI();
    }
    this->ExchangeTimer();
}
void PlayingFace::on_ai_finish(int Row, int Column)
{
    int NowPlayer = GM->getNowPlayer();
    GM->downChess(Row, Column, NowPlayer);
    chess_board->DownChess(Row, Column, NowPlayer);
    GM->exchangePlayer();//交换棋手
    if(GM->deadChess(Row, Column))
    {
        this->ShowWinner(GM->getNowPlayer());
        return;
    }
    this->ExchangeTimer();
}
//开始游戏按钮事件
void PlayingFace::on_pushButton_clicked()
{
    if(ui->pushButton->text() == QMainWindow::tr("开始游戏"))
    {
        InitData();
        ui->pushButton->setText(QMainWindow::tr("暂停游戏"));
        InGaming = true;
        pauseTimeLength = clock();
        player1Timer->setInterval(1000);
        player1Timer->start();
        if(type == 1) this->RunAI();
    }
    else if(ui->pushButton->text() == QMainWindow::tr("暂停游戏"))
    {
        if(type == 4) NetworkTool::sendMessage(QMainWindow::tr("14"), NT->getRivalIP());
        else PauseGame();
    }
    else if(ui->pushButton->text() == QMainWindow::tr("继续游戏"))
    {
        if(type == 4) NetworkTool::sendMessage(QMainWindow::tr("15"), NT->getRivalIP());
        ContinueGame();
    }
    else if(ui->pushButton->text() == QMainWindow::tr("返回房间"))
    {
        this->hide();
        last->show();
    }
}
void PlayingFace::ContinueGame()
{
    ui->pushButton->setText(QMainWindow::tr("暂停游戏"));
    if(GM->getNowPlayer() == 1)
    {
        player1Timer->setInterval(1000 - pauseTimeLength);
        player1Timer->start();
    }
    else
    {
        player2Timer->setInterval(1000 - pauseTimeLength);
        player2Timer->start();
    }
}
void PlayingFace::PauseGame()
{
    player1Timer->stop();
    player2Timer->stop();
    pauseTimeLength = clock() - pauseTimeLength;
    ui->pushButton->setText(QMainWindow::tr("继续游戏"));
}
void PlayingFace::BackPace()
{
    int r, c;
    GM->backPace(r, c);
    if(r == 0 && c == 0) return;
    chess_board->backPace(r, c);
    GM->exchangePlayer();//交换棋手
    this->ExchangeTimer();
    POSITION *temp = GM->getLastPace();
    chess_board->setLastPace(temp->row, temp->column);
    chess_board->setLastChessRed(temp->row, temp->column);
}
void PlayingFace::ShowWinner(int winner)
{
    player1Timer->stop();
    player2Timer->stop();
    QString win = (winner == 1 ? QMainWindow::tr("（黑子）") : QMainWindow::tr("（白子）")) + QMainWindow::tr("获胜");
    QMessageBox::information(this, "胜负已分", "恭喜玩家" + QString::number(winner) + win);
    if(type == 4) ui->pushButton->setText(QMainWindow::tr("返回房间"));
    else ui->pushButton->setText(QMainWindow::tr("开始游戏"));
}
void PlayingFace::ExchangeTimer()
{
    if(GM->getNowPlayer() == 1)
    {
        this->player2RoundTime = GameMessage::getRoundTime();
        player1Timer->stop();
        player2Timer->setInterval(1000);
        player2Timer->start();
    }
    else
    {
        this->player1RoundTime = GameMessage::getRoundTime();
        player2Timer->stop();
        player1Timer->setInterval(1000);
        player1Timer->start();
    }
}
void PlayingFace::on_udpSocket_receive()
{
    QHostAddress sender;
    QByteArray datagram;
    QUdpSocket *receiver = NT->getReceiver();
    datagram.resize(receiver->pendingDatagramSize());
    receiver->readDatagram(datagram.data(), datagram.size(), &sender);
    QString msg = datagram;
    QList<QString> message = msg.split(" ");
    int msgType = message.at(0).toInt();
    switch(msgType)
    {
    case 10:
    {
        QList<QString> str = sender.toString().split(":");
        QString localip = str.at(str.size() - 1);
        int downRow = message.at(1).toInt(), downColumn = message.at(2).toInt();
        int NowPlayer = GM->getNowPlayer();
        if(localip != NT->getLocalIP()) GM->downChess(downRow, downColumn, NowPlayer);
        chess_board->DownChess(downRow, downColumn, NowPlayer);
        GM->exchangePlayer();//交换棋手
        if(GM->deadChess(downRow, downColumn))
        {
            this->ShowWinner(GM->getNowPlayer());
            return;
        }
        this->ExchangeTimer();
    };break;
    case 11://接收到“请求悔棋消息”
    {
        int select = QMessageBox::information(this, QMainWindow::tr("请求"), QMainWindow::tr("对方请求悔棋，是否同意？"), QMessageBox::Ok, QMessageBox::Cancel);
        if(select == QMessageBox::Ok)
        {
            NetworkTool::sendMessage(QMainWindow::tr("13"), NT->getLocalIP());
            NetworkTool::sendMessage(QMainWindow::tr("13"), NT->getRivalIP());
        }
    };break;
    case 12:ShowWinner(3 - message.at(1).toInt());break;//接收到“认输消息”
    case 13:BackPace();break;//接收到“同意悔棋消息”
    case 14://接收到“请求暂停消息”
    {
        int select = QMessageBox::information(this, QMainWindow::tr("请求"), QMainWindow::tr("对方请求暂停，是否同意？"), QMessageBox::Ok, QMessageBox::Cancel);
        if(select == QMessageBox::Ok)
        {
            NetworkTool::sendMessage(QMainWindow::tr("16"), NT->getLocalIP());
            NetworkTool::sendMessage(QMainWindow::tr("16"), NT->getRivalIP());
        }
    };break;
    case 15:ContinueGame();break;//接收到“继续游戏消息”
    case 16:PauseGame();break;//接收到“同意暂停消息”
    case 17:ShowWinner(GM->getNowPlayer());break;//接收到“超时获胜消息”
    }
}
void Thread::run()
{
    GameMessage::AI.getPosition(row, column, myTurn);
    emit ai_finish(row, column);
}
