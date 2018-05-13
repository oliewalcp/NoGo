#ifndef GAMEHELPERFACE_H
#define GAMEHELPERFACE_H

#include <QWidget>

namespace Ui {
class GameHelperFace;
}

class GameHelperFace : public QWidget
{
    Q_OBJECT

public:
    explicit GameHelperFace(QWidget *parent = 0);
    ~GameHelperFace();
    void Enter(QWidget *last);

private slots:
    void on_pushButton_clicked();

private:
    Ui::GameHelperFace *ui;
    QWidget *lastPanel;
};

#endif // GAMEHELPERFACE_H
