#ifndef USERPAGE_H
#define USERPAGE_H

#include <QDialog>
#include"UserController.h"
#include"PlayerInfo.h"
#include"editprofile.h"
#include"GamePage.h"
namespace Ui {
class UserPage;
}

class UserPage : public QDialog
{
    Q_OBJECT

public:
    explicit UserPage(UserController*,PlayerInfo*,QWidget *parent = nullptr);
    ~UserPage();

protected:
    void showEvent(QShowEvent *event) override;
private slots:
    void on_btnExit_clicked();

    void on_btnEdit_clicked();

    void on_btnStart_clicked();

    void onGameReady(const QJsonObject &gameInfo);

private:
    Ui::UserPage *ui;
    UserController* _controller;
    PlayerInfo* _player;
    EditProfile* _edit;
};

#endif // USERPAGE_H
