#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QString name, QString percent, bool host, QWidget *parent = 0);
    ~Player();

    void changeStatus();
    QString toString();
    bool getStatus();

private:
    Ui::Player *ui;
};

#endif // PLAYER_H
