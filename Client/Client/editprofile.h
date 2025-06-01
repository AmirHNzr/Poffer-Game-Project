#ifndef EDITPROFILE_H
#define EDITPROFILE_H

#include <QDialog>
#include"UserController.h"
#include"PlayerInfo.h"
namespace Ui {
class EditProfile;
}

class EditProfile : public QDialog
{
    Q_OBJECT

public:
    explicit EditProfile(UserController*,PlayerInfo*,QWidget *parent = nullptr);
    ~EditProfile();

private slots:

private:
    Ui::EditProfile *ui;
    UserController* _controller;
    PlayerInfo* _player;
};

#endif // EDITPROFILE_H
