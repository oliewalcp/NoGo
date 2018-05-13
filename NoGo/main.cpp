#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("utf-8");//设置编码格式
    QTextCodec::setCodecForLocale(codec);
    MainWindow w;
    w.show();
    return a.exec();
}
