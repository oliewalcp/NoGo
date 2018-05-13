#include "mainmenuface.h"
#include "ui_mainmenuface.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QFile>
#include <QSettings>

MainMenuFace::MainMenuFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenuFace)
{
    ui->setupUi(this);
    this->hide();
}
void MainMenuFace::Enter(QVBoxLayout *layout)
{
    this->rootLayout = layout;
    gameSetter = new GameSetterFace(this);
    gameHelper = new GameHelperFace(this);
    playingFace = new PlayingFace(this);
    waitArea = new WaitingArea(this);
    innerRoom = new InnerRoom(this);
    layout->addWidget(playingFace);
    layout->addWidget(gameSetter);
    layout->addWidget(gameHelper);
    layout->addWidget(waitArea);
    layout->addWidget(innerRoom);
    playingFace->hide();
    gameSetter->hide();
    gameHelper->hide();
    waitArea->hide();
    innerRoom->hide();
    this->show();
}
MainMenuFace::~MainMenuFace()
{
    delete ui;
}
//单人游戏按钮
void MainMenuFace::on_pushButton_clicked()
{
    if(GameMessage::lib != nullptr)
    {
        GameMessage::lib->unload();
        delete GameMessage::lib;
        GameMessage::lib = nullptr;
    }
    readSetData();
    if(!GameMessage::getLibStatus(GameMessage::getAIFile())) return;
    else GameMessage::getLibStatus(GameMessage::getAIFile(), false);
    int limitRow = 1, limitColumn = 1;
    GameMessage::AI.get_limit(limitRow, limitColumn);//获取AI限制的棋盘大小
    //如果AI限制的棋盘大小与用户设置的不同
    if((limitRow != GameMessage::getRow() && limitRow != 0) || (limitColumn != GameMessage::getColumn() && limitColumn != 0))
    {
        if(QMessageBox::information(this, QMainWindow::tr("冲突"), QMainWindow::tr("AI限制的棋盘大小与设置中的棋盘大小冲突，是否修\n改棋盘大小为AI的限制大小？"),
                                    QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            GameMessage::setChessBoardSize(limitRow, limitColumn);
        else
        {
            GameMessage::lib->unload();
            delete GameMessage::lib;
            GameMessage::lib = nullptr;
            return;
        }
    }
    if(QMessageBox::information(this, QMainWindow::tr("请选择"), QMainWindow::tr("你是否执黑子（先手）？"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        this->hide();
        playingFace->Enter(this, 2);
    }
    else
    {
        this->hide();
        playingFace->Enter(this, 1);
    }
}
//双人游戏按钮
void MainMenuFace::on_pushButton_2_clicked()
{
    readSetData();
    this->hide();
    playingFace->Enter(this, 0);
}
//游戏帮助按钮
void MainMenuFace::on_pushButton_3_clicked()
{
    this->hide();
    gameHelper->Enter(this);
}
//游戏设置按钮
void MainMenuFace::on_pushButton_4_clicked()
{
    readSetData();
    this->hide();
    gameSetter->Enter(this);
}
//退出游戏按钮
void MainMenuFace::on_pushButton_5_clicked()
{
    exit(0);
}
//联机游戏按钮
void MainMenuFace::on_pushButton_11_clicked()
{
    this->hide();
    if(!waitArea->Enter(this, innerRoom))
    {
        this->show();
        QMessageBox::information(this, QMainWindow::tr("网络有点问题"), QMainWindow::tr("网络连接失败"));
    }
}
void MainMenuFace::readSetData()
{
    QSettings *setfile = new QSettings("sdat.ini", QSettings::IniFormat);
    QString row = setfile->value("li").toString();
    QString col = setfile->value("co").toString();
    if(row != "" && col != "") GameMessage::setChessBoardSize(row.toInt(), col.toInt());
    else GameMessage::setChessBoardSize(9, 9);
    row = setfile->value("tt").toString();
    col = setfile->value("rt").toString();
    if(row != "" && col != "") GameMessage::setGameTime(row.toInt(), col.toInt());
    else GameMessage::setGameTime(900, 60);
    row = setfile->value("ai").toString();
    if(row != "") GameMessage::setAIFile(row);
    else GameMessage::setAIFile(QCoreApplication::applicationDirPath() + QMainWindow::tr("/AI/AI.dll"));
    delete setfile;
}
