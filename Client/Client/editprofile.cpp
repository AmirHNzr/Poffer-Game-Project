#include "editprofile.h"
#include "ui_editprofile.h"

EditProfile::EditProfile(UserController* control,PlayerInfo* p,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditProfile)
{
    ui->setupUi(this);
    _player = p;
    _controller = control;

    connect(ui->btnEdit, &QPushButton::clicked,
            this, [=]() {
                QString user = _player->username();
                QString fn = ui->lnFname->text();
                QString ln = ui->lnLname->text();
                QString num = ui->lnPhone->text();
                QString email = ui->lnEmail->text();
                QString user2 = ui->lnUsername->text();
                QString pass = ui->lnPassword->text();
                _controller->EditProfile(user,fn,ln,num,email,user2,pass);
            });

    connect(_controller,&UserController::EditPermission,this,[&](){_player->setUsername(ui->lnUsername->text());});
    ui->lnPassword->setEnabled(true);
}

EditProfile::~EditProfile()
{
    delete ui;
}




