#ifndef GAMESETTERFACE_H
#define GAMESETTERFACE_H

#include <QWidget>
#include <QString>
#include "gamemessage.h"
namespace Ui {
class GameSetterFace;
}

class GameSetterFace : public QWidget
{
    Q_OBJECT

public:
    explicit GameSetterFace(QWidget *parent = 0);
    ~GameSetterFace();
    void Enter(QWidget *last);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    QWidget *lastPanel;
    Ui::GameSetterFace *ui;

    void writeFile();
    void readFile();
};

#endif // GAMESETTERFACE_H
