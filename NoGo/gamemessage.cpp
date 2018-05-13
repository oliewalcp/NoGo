#include "gamemessage.h"
#include <QDebug>
#include <QHostInfo>
#include <QMainWindow>
#include <QMessageBox>

int GameMessage::row = 9;
int GameMessage::column = 9;
unsigned int GameMessage::TotalTime = 900;
unsigned int GameMessage::RoundTime = 60;
unsigned int GameMessage::chessWidth = 0;
unsigned int GameMessage::chessHeight = 0;
QString GameMessage::SingleAI = QMainWindow::tr("/AI/AI.dll");
AI_FUNCTION GameMessage::AI;
QLibrary* GameMessage::lib = nullptr;

GameMessage::GameMessage()
{
    Init();
}
void GameMessage::exchangePlayer()
{
    NowPlayer = 3 - NowPlayer;
}
void GameMessage::setChessBoardSize(int Row, int Column)
{
    row = Row;
    column = Column;
}
void GameMessage::setGameTime(int totalTime, int roundTime)
{
    TotalTime = totalTime;
    RoundTime = roundTime;
}
void GameMessage::setChessSize(int width, int height)
{
    chessWidth = width;
    chessHeight = height;
}
int GameMessage::getGas(int Row, int Column, const int who, std::list<POSITION *> *FreePos)
{
    if(FreePos == nullptr)
        FreePos = new std::list<POSITION*>;
    return getGas(this->cross, Row, Column, who, FreePos);
}
void GameMessage::Init()
{
    Init(this->cross);
    NowPlayer = 1;
    while(!downChessMemory.empty())
    {
        delete downChessMemory.back();
        downChessMemory.pop_back();
    }
}
void GameMessage::Init(Cross &cross)
{
    cross.clear();
    int i = 1, j = 1;
    int rowNumber = row + 1;
    int columnNumber = column + 1;
    for(i = 1; i < rowNumber; i++)
    {
        std::unordered_map<int, int> *second = new std::unordered_map<int, int>;
        for(j = 1; j < columnNumber; j++)
            (*second)[j] = 0;
        cross[i] = second;
    }
}
bool GameMessage::downChess(int Row, int Column, int Player)
{
    if(Row > row || Row < 1 || Column < 1 || Column > column || (*(cross[Row]))[Column] != 0) return false;//非法位置下棋失败
    (*(cross[Row]))[Column] = Player;
    POSITION* pos = new POSITION(Row, Column);
    downChessMemory.push_back(pos);
    return true;
}
void GameMessage::backPace(int &Row, int &Column)
{
    Row = Column = 0;
    if(downChessMemory.empty()) return;
    POSITION *pos = downChessMemory.back();
    downChessMemory.pop_back();
    Row = pos->row;
    Column = pos->column;
    (*(cross[Row]))[Column] = 0;
    delete pos;
}
/* 获取某个位置上的气
 * param[MyCross]:棋盘上的落子状态
 * param[Row]:目标位置——行
 * param[Column]:目标位置——列
 * param[who]:谁的棋子
 * param[FreePos]:还剩余的气的所在位置（就是空位），如果气大于4，则只返回4个位置
 * return:返回气的大小，当气大于4时，返回-1
 */
int GameMessage::getGas(Cross &MyCross, int Row, int Column, const int who, std::list<POSITION *> *FreePos)
{
    if(FreePos == nullptr)
        FreePos = new std::list<POSITION*>;
    Cross visit;//棋盘访问标记
    Init(visit);//初始化
    int Rival = 3 - who;
    (*(visit[Row]))[Column] = 1;//设置访问标记
    if((*(MyCross[Row]))[Column] == 0)
        FreePos->push_back(new POSITION(Row, Column));
    if(notHaveChess(MyCross, visit, Row - 1, Column, Rival, FreePos) &&
            notHaveChess(MyCross, visit, Row + 1, Column, Rival, FreePos) &&
            notHaveChess(MyCross, visit, Row, Column + 1, Rival, FreePos) &&
            notHaveChess(MyCross, visit, Row, Column - 1, Rival, FreePos))
    {
        int gas = FreePos->size();
        return gas > 4 ? -1 : gas;
    }
    int gas = FreePos->size();
    return gas > 4 ? -1 : gas;
}
/* 判断某个位置上是否没有自己的棋子
 * param[MyCross]:棋盘上的落子状态
 * param[visit]:遍历情况
 * param[Row]:目标位置——行
 * param[Column]:目标位置——列
 * param[Rival]:对方的棋子
 * param[FreePos]:还剩余的气的所在位置（就是空位），如果气大于4，则只返回4个位置
 * return:当该位置上有自己的棋子时，返回false，否则返回true
 */
bool GameMessage::notHaveChess(Cross &MyCross, Cross &visit, int Row, int Column, const int Rival, std::list<POSITION *> *FreePos)
{
    if(Row > row || Row < 1 || Column < 1 || Column > column) return true;//超出棋盘空间，返回true
    if((*(visit[Row]))[Column] == 1) return true;//已遍历，返回true
    if((*(MyCross[Row]))[Column] == Rival) return true;//该位置有对方的棋子，返回true
    (*(visit[Row]))[Column] = 1;
    if((*(MyCross[Row]))[Column] == 0)
    {
        FreePos->push_back(new POSITION(Row, Column));
        return true;
    }
    if(notHaveChess(MyCross, visit, Row - 1, Column, Rival, FreePos) &&
            notHaveChess(MyCross, visit, Row + 1, Column, Rival, FreePos) &&
            notHaveChess(MyCross, visit, Row, Column + 1, Rival, FreePos) &&
            notHaveChess(MyCross, visit, Row, Column - 1, Rival, FreePos))
        return true;
    return false;
}
/* 判断某个位置上的棋子是否是死棋
 * param[Row]:目标位置——行
 * param[Column]:目标位置——列
 * return:如果该位置没有棋子，返回false，如果该位置上的棋子是死棋，返回true，否则返回false
 */
bool GameMessage::deadChess(int Row, int Column)
{
    int who = (*(cross[Row]))[Column], rival = 3 - who;
    if(who == 0) return false;//该位置没有棋子
    if(getGas(Row, Column, who) == 0) return true;//当前位置的棋子是死棋
    auto GetGas = [&](int Row, int Column) -> bool {
        if(Row > row || Row < 1 || Column < 1 || Column > column) return false;//超出棋盘空间
        if((*(cross[Row]))[Column] != rival) return false;
        if(getGas(Row, Column, rival) == 0) return true;
        return false;
    };
    if(GetGas(Row - 1, Column) || GetGas(Row + 1, Column) || GetGas(Row, Column - 1) || GetGas(Row, Column + 1)) return true;
    return false;
}


void NetworkTool::sendMessage(QString MesData, QString IPAddress)
{
    QUdpSocket *send = new QUdpSocket();
    QByteArray datagram = MesData.toLatin1();
    send->writeDatagram(datagram.data(), datagram.size(), IPAddress == "" ? QHostAddress::Broadcast : QHostAddress(IPAddress), PORTNUMBER);
    delete send;
}
QString NetworkTool::getLocalMachineIP()
{
    QString Local_IP = "";
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    //如果存在多条ip地址ipv4和ipv6：
    for(QHostAddress address : info.addresses())
    {
        //只取ipv4协议的地址
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return Local_IP;
}
bool GameMessage::getLibStatus(QString filename, bool needUnLoad)
{
    lib = new QLibrary(filename);
    bool success = true;
    if(lib->load())
    {
        GameMessage::AI.getLimit = (GetLimit)(lib->resolve("getLimit"));
        GameMessage::AI.initData = (InitData)(lib->resolve("initData"));
        GameMessage::AI.getPosition = (GetPosition)(lib->resolve("getPosition"));
        if(!GameMessage::AI.getLimit || !GameMessage::AI.initData || !GameMessage::AI.getPosition)
        {
            QMessageBox::information(0, QMainWindow::tr("抱歉"), QMainWindow::tr("加载AI文件失败"));
            success = false;
        }
        else
        {
            int test1 = 0, test2 = 0;
            GameMessage::AI.get_limit(test1, test2);
            if(test1 != -1 || test2 != -1)
            {
                QMessageBox::information(0, QMainWindow::tr("加载失败"), QMainWindow::tr("getLimit函数未完整"));
                success = false;
            }
            else
            {
                if(!GameMessage::AI.init_data((test1 = 0), (test2 = 0), 0))
                {
                    QMessageBox::information(0, QMainWindow::tr("加载失败"), QMainWindow::tr("initData函数未完整"));
                    success = false;
                }
                else
                {
                    GameMessage::AI.get_position((test1 = 0), (test2 = 0), 0);
                    if(test1 != -1 || test2 != -1)
                    {
                        QMessageBox::information(0, QMainWindow::tr("加载失败"), QMainWindow::tr("getPosition函数未完整"));
                        success = false;
                    }
                }
            }
        }
        if(needUnLoad)
        {
            lib->unload();
            delete lib;
            lib = nullptr;
        }
    }
    else
    {
        QMessageBox::information(0, QMainWindow::tr("抱歉"), QMainWindow::tr("加载AI文件失败"));
        success = false;
    }
    return success;
}
