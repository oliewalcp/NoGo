#include "gamehelperface.h"
#include "ui_gamehelperface.h"

GameHelperFace::GameHelperFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameHelperFace)
{
    ui->setupUi(this);
    ui->label_2->setWordWrap(true);
    ui->label_3->setWordWrap(true);
    ui->label_4->setWordWrap(true);
    ui->label_5->setWordWrap(true);
    ui->label_6->setWordWrap(true);
}

GameHelperFace::~GameHelperFace()
{
    delete ui;
}

void GameHelperFace::on_pushButton_clicked()
{
    this->hide();
    lastPanel->show();
}
void GameHelperFace::Enter(QWidget *last)
{
    this->lastPanel = last;
    last->hide();
    this->show();
}
