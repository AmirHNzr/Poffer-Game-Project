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


void EditProfile::Regex()
{
    connect(ui->lnEmail, &QLineEdit::editingFinished, this, [this]() {
        const QString text = ui->lnEmail->text();
        static const QRegularExpression re(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
        if (!re.match(text).hasMatch()) {
            QMessageBox::warning(this,
                                 tr("Invalid Email"),
                                 tr("“%1” is not a valid email address.").arg(text));
            ui->lnEmail->clear();
            ui->lnEmail->setFocus();
        }
    });

    connect(ui->lnFname, &QLineEdit::editingFinished, this, [this]() {
        const QString text = ui->lnFname->text();
        static const QRegularExpression nameRegex("^[a-zA-Z][a-zA-Z]*(?: [A-Z][a-zA-Z]*)*$");

        if (!nameRegex.match(text).hasMatch()) {
            QMessageBox::warning(this,
                                 tr("Invalid First Name"),
                                 tr("“%1” is not a valid name.").arg(text));
            ui->lnFname->clear();
            ui->lnFname->setFocus();
        }
    });

    connect(ui->lnLname, &QLineEdit::editingFinished, this, [this]() {
        const QString text = ui->lnLname->text();
        static const QRegularExpression nameRegex("^[a-zA-Z][a-zA-Z]*(?: [A-Z][a-zA-Z]*)*$");

        if (!nameRegex.match(text).hasMatch()) {
            QMessageBox::warning(this,
                                 tr("Invalid Last Name"),
                                 tr("“%1” is not a valid name.").arg(text));
            ui->lnLname->clear();
            ui->lnLname->setFocus();
        }
    });

    connect(ui->lnUsername, &QLineEdit::editingFinished, this, [this]() {
        const QString text = ui->lnUsername->text();
        static const QRegularExpression usernameRegex("^[a-zA-Z][a-zA-Z0-9_]{2,14}$");

        if (!usernameRegex.match(text).hasMatch()) {
            QMessageBox::warning(this,
                                 tr("Invalid Username"),
                                 tr("“%1” is not a valid Username.").arg(text));
            ui->lnUsername->clear();
            ui->lnUsername->setFocus();
        }
    });

    connect(ui->lnPassword, &QLineEdit::editingFinished, this, [this]() {
        const QString text = ui->lnPassword->text();
        static const QRegularExpression strongPasswordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).{7,}$");
        if (!strongPasswordRegex.match(text).hasMatch()) {
            QMessageBox::warning(this,
                                 tr("Invalid Password"),
                                 tr("Passwords should contain at least:\nOne LowerCase and One Uppercase character\n"
                                    "One special character\nOne digit\nand it should be at least 7 characters"));
            ui->lnPassword->clear();
            ui->lnPassword->setFocus();
        }
    });


}


EditProfile::~EditProfile()
{
    delete ui;
}





void EditProfile::on_btnExit_clicked()
{
    close();
}

