#include "UserPage.h"
#include "ui_UserPage.h"

UserPage::UserPage(UserController* control,PlayerInfo* p,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserPage)
{
    ui->setupUi(this);
    _controller = control;
    _player = p;
}

UserPage::~UserPage()
{
    delete ui;
}

void UserPage::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

}

void UserPage::on_btnExit_clicked()
{
    this->close();
}


