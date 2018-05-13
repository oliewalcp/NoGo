#ifndef GAMEMESSAGE_H
#define GAMEMESSAGE_H

#include <unordered_map>
#include <list>
#include <QString>
#include <QUdpSocket>
#include <QLibrary>

#define PORTNUMBER 23333
typedef std::unordered_map<int, std::unordered_map<int, int>*> Cross;//行——列——玩家编号
typedef void(*GetLimit)(int &, int &);
typedef bool(*InitData)(int, int, int);
typedef void(*GetPosition)(int &, int &, int);

struct AI_FUNCTION
{
    GetLimit getLimit;
    InitData initData;
    GetPosition getPosition;
    void get_limit(int &LimitRow, int &LimitColumn){getLimit(LimitRow, LimitColumn);}
    bool init_data(int BoardRow, int BoardColumn, int player){return initData(BoardRow, BoardColumn, player);}
    void get_position(int &Row, int &Column, int player){getPosition(Row, Column, player);}
};
//棋盘坐标
class POSITION
{
public:
    int row;//行
    int column;//列
    POSITION () {}
    POSITION (int Row, int Column)
    {
        row = Row;
        column = Column;
    }
};
//网络通信工具
class NetworkTool
{
private:
    QString RivalIP;//对手IP地址
    QString LocalIP;//本地IP地址
    QString LocalName;//本机的游戏名称
    QString LocalPercent;//本机的胜率
    QUdpSocket *receiver;//接收工具
public:
    NetworkTool(){LocalName = "";LocalPercent = "0";}
    QString getRivalIP(){return RivalIP;}
    QString getLocalIP(){return LocalIP;}
    QString getLocalName(){return LocalName;}
    QString getLocalPercent(){return LocalPercent;}
    QUdpSocket* getReceiver(){return receiver;}
    void setRivalIP(QString ip){RivalIP = ip;}
    void setLocalIP(QString ip){LocalIP = ip;}
    void setPersonalMessage(QString name, QString percent){this->LocalName = name;this->LocalPercent = percent;}
    void setRecevier(QUdpSocket *rec){receiver = rec;}
    void stopReceiver(){receiver->close();delete receiver;}
    static void sendMessage(QString MesData, QString IPAddress = "");
    static QString getLocalMachineIP();
};
//游戏信息类
class GameMessage
{
private :
    static QString SingleAI;//单人模式下的AI文件路径
    static int row;//行数
    static int column;//列数
    static unsigned int TotalTime;//游戏局时
    static unsigned int RoundTime;//游戏步时
    static unsigned int chessWidth;//棋子的宽度
    static unsigned int chessHeight;//棋子的高度
    int NowPlayer;//现在轮到的玩家
    std::list<POSITION*> downChessMemory;//下棋记录
    Cross cross;//记录棋盘上各个落子点的棋子
    static bool notHaveChess(Cross &MyCross, Cross &visit, int Row, int Column, const int who, std::list<POSITION*> *FreePos);

public :
    GameMessage();
    static AI_FUNCTION AI;//AI对象
    static QLibrary *lib;
    static bool getLibStatus(QString filename, bool needUnLoad = true);//获取库文件的状态
    static void setChessBoardSize(int Row, int Column);//设置棋盘规模
    static void setGameTime(int totalTime, int roundTime);//设置游戏时间
    static void setChessSize(int width, int height);//设置棋子规模
    static void setAIFile(QString file){SingleAI = file;}//设置AI文件路径
    static QString getAIFile(){return SingleAI;}//获取AI文件路径
    static int getTotalTime(){ return TotalTime; }//获取游戏局时
    static int getRoundTime(){ return RoundTime; }//获取游戏步时
    static int getRow(){ return row; }//获取棋盘的行数
    static int getColumn(){ return column; }//获取棋盘的列数
    static int getChessWidth(){ return chessWidth; }//获取棋子的宽度
    static int getChessHeight(){ return chessHeight; }//获取棋子的高度
    POSITION* getLastPace(){return downChessMemory.back();}
    static int getGas(Cross &MyCross, int Row, int Column, const int who, std::list<POSITION*> *FreePos = nullptr);//获取某个位置的气
    static void Init(Cross &cross);//初始化
    int getGas(int Row, int Column, const int who, std::list<POSITION*> *FreePos = nullptr);//获取某个位置的气
    void Init();
    bool downChess(int Row, int Column, int Player);//下棋记录
    void backPace(int &Row, int &Column);//悔棋
    int getNowPlayer(){ return NowPlayer; }//获取现在轮到的玩家
    void exchangePlayer();//交换棋手
    bool deadChess(int Row, int Column);//判断某位置上的棋子是否是死棋
};
#endif // GAMEMESSAGE_H
