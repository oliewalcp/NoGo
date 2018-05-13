#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resource/Board.bmp"));
    MainMenuFace *mmf = new MainMenuFace(this);
    ui->rootLayout->addWidget(mmf);
    mmf->Enter(ui->rootLayout);
}
MainWindow::~MainWindow()
{
    delete ui;
}
