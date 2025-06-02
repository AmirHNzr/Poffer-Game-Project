#include "UserPage.h"
#include "ui_UserPage.h"

UserPage::UserPage(UserController* control,PlayerInfo* p,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserPage)
{
    ui->setupUi(this);
    _controller = control;
    _player = p;
    _edit = new EditProfile(_controller,_player);

    connect(ui->btnHistory, &QPushButton::clicked,
                            this, [=]() {
                            QString user = _player->username();
                            _controller->GetHistory(user);
                            });

    connect(_controller, &UserController::GameReady,this, &UserPage::onGameReady);

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



void UserPage::on_btnEdit_clicked()
{
    _edit->show();
}


void UserPage::on_btnStart_clicked()
{
    if (!_controller->isConnected()) {
        QMessageBox::warning(this, "Connection Error", "Not connected to server.");
        return;
    }

    //a JSON object that tells the server “I want to join the queue.”
    QJsonObject request;
    request["cmd"]      = "JOIN_QUEUE";
    request["username"] = _player->username();

    _controller->sendJson(request);

    // Disable the button. change its text to “Waiting…”
    ui->btnStart->setEnabled(false);
    ui->btnStart->setText("Waiting for players...");
}

void UserPage::onGameReady(const QJsonObject &gameInfo)
{
    QJsonArray players = gameInfo["players"].toArray();

    QMessageBox::information(this,
                             "Game Ready!",
                             QString("Enough players joined. Starting game now."));

    // Instantiate GamePage
    // Pass: player info, opponent, etc.
    GamePage *gp = new GamePage(_player, players, this);
    gp->show();
    this->hide();
}

