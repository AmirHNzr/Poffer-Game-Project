#include "GamePage.h"
#include "ui_GamePage.h"

GamePage::GamePage(PlayerInfo* p,QJsonArray ps,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GamePage)
    , _players(ps)
{
    ui->setupUi(this);
    _player = p;
}

GamePage::~GamePage()
{
    delete ui;
}
