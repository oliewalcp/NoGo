#include "gamesetterface.h"
#include "ui_gamesetterface.h"
#include <QFile>
#include <QSettings>
#include <QFileDialog>
#include <QMainWindow>

GameSetterFace::GameSetterFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameSetterFace)
{
    ui->setupUi(this);
    QValidator *val = new QIntValidator(2, 200000000, this);//设定输入框的输入范围
    ui->TotalTimeInput->setValidator(val);
    ui->RoundTimeInput->setValidator(val);
    ui->ColumnNumber->setValidator(val);
    ui->RowNumber->setValidator(val);
}

GameSetterFace::~GameSetterFace()
{
    delete ui;
}
//确定按钮
void GameSetterFace::on_pushButton_clicked()
{
    GameMessage::setChessBoardSize(ui->RowNumber->text().toInt(), ui->ColumnNumber->text().toInt());
    GameMessage::setGameTime(ui->TotalTimeInput->text().toInt(), ui->RoundTimeInput->text().toInt());
    this->writeFile();
    on_pushButton_2_clicked();
}
//取消按钮
void GameSetterFace::on_pushButton_2_clicked()
{
    this->hide();
    lastPanel->show();
}
void GameSetterFace::writeFile()
{
    QFile::remove("sdat.ini");
    QSettings *setfile = new QSettings("sdat.ini", QSettings::IniFormat);
    setfile->setValue("li", ui->RowNumber->text());
    setfile->setValue("co", ui->ColumnNumber->text());
    setfile->setValue("tt", ui->TotalTimeInput->text());
    setfile->setValue("rt", ui->RoundTimeInput->text());
    setfile->setValue("ai", ui->label_6->text());
    delete setfile;
}
void GameSetterFace::readFile()
{
    QSettings *setfile = new QSettings("sdat.ini", QSettings::IniFormat);
    QString str = setfile->value("li").toString();
    if(str != "") ui->RowNumber->setText(str);
    str = setfile->value("co").toString();
    if(str != "") ui->ColumnNumber->setText(str);
    str = setfile->value("tt").toString();
    if(str != "") ui->TotalTimeInput->setText(str);
    str = setfile->value("rt").toString();
    if(str != "") ui->RoundTimeInput->setText(str);
    str = setfile->value("ai").toString();
    if(str != "")
    {
        ui->label_6->setText(str);
        if(str != QMainWindow::tr("默认")) GameMessage::setAIFile(str);
        else GameMessage::setAIFile(QCoreApplication::applicationDirPath() + QMainWindow::tr("/AI/AI.dll"));
    }
    delete setfile;
}
void GameSetterFace::Enter(QWidget *last)
{
    ui->TotalTimeInput->setText(QString::number(GameMessage::getTotalTime()));
    ui->RoundTimeInput->setText(QString::number(GameMessage::getRoundTime()));
    ui->ColumnNumber->setText(QString::number(GameMessage::getColumn()));
    ui->RowNumber->setText(QString::number(GameMessage::getRow()));
    readFile();
    ui->label_6->setText(QCoreApplication::applicationDirPath() + QMainWindow::tr("/AI/AI.dll"));
    this->lastPanel = last;
    last->hide();
    this->show();
}
//选择AI文件按钮
void GameSetterFace::on_pushButton_3_clicked()
{
    //选择一个文件
    QString filename = QFileDialog::getOpenFileName(
        this,
        QMainWindow::tr("选择一个AI文件"),
        QMainWindow::tr("/"),
        QMainWindow::tr("AI (*.dll *.so)"));
    if(filename.isEmpty()) return;
    if(GameMessage::getLibStatus(filename))
        ui->label_6->setText(filename);
}
//恢复默认按钮事件
void GameSetterFace::on_pushButton_4_clicked()
{
    ui->label_6->setText(QMainWindow::tr("默认"));
    GameMessage::setAIFile(QCoreApplication::applicationDirPath() + QMainWindow::tr("/AI/AI.dll"));
}
