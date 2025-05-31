#ifndef USERPAGE_H
#define USERPAGE_H

#include <QDialog>
#include"UserController.h"
#include"PlayerInfo.h"
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

private:
    Ui::UserPage *ui;
    UserController* _controller;
    PlayerInfo* _player;
};

#endif // USERPAGE_H
