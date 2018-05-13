#ifndef MAINMENUFACE_H
#define MAINMENUFACE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "gamesetterface.h"
#include "gamehelperface.h"
#include "playingface.h"
#include "waitingarea.h"
#include "innerroom.h"

namespace Ui {
class MainMenuFace;
}

class MainMenuFace : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuFace(QWidget *parent = 0);
    ~MainMenuFace();

    void Enter(QVBoxLayout *layout);
    QWidget* getPlayingFace() {return playingFace;}

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::MainMenuFace *ui;
    QVBoxLayout *rootLayout;
    GameSetterFace *gameSetter;
    GameHelperFace *gameHelper;
    PlayingFace *playingFace;
    WaitingArea *waitArea;
    InnerRoom *innerRoom;

    void readSetData();
};

#endif // MAINMENUFACE_H
