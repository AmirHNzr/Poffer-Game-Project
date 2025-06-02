#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QDialog>
#include"PlayerInfo.h"
#include<QJsonArray>
namespace Ui {
class GamePage;
}

class GamePage : public QDialog
{
    Q_OBJECT

public:
    explicit GamePage(PlayerInfo*,QJsonArray,QWidget *parent = nullptr);
    ~GamePage();

private:
    Ui::GamePage *ui;
    PlayerInfo* _player;
    QJsonArray _players;
};

#endif // GAMEPAGE_H
