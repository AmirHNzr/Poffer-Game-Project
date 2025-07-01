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
    _hist = new HistoryPage();

    connect(ui->btnHistory, &QPushButton::clicked,
                            this, [=]() {
                            QString user = _player->username();
                            _controller->GetHistory(user);
                            });

    connect(_controller, &UserController::GameReady,this, &UserPage::onGameReady);

    CreateFont();


}


UserPage::~UserPage()
{
    delete ui;
}

void UserPage::CreateFont(){
    int fontId = QFontDatabase::addApplicationFont(":/Font/balatro.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont customFont(family, 16, true);
    _font = customFont;

}

void UserPage::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    ui->wlcLbl->setFont(_font);
    ui->wlcLbl->setText("Welcome "+_player->username());
}

void UserPage::on_btnExit_clicked()
{
    hist.clear();
    // this->close();
    ui->btnStart->setEnabled(true);
    ui->infoLbl->clear();

    accept();
}



void UserPage::on_btnEdit_clicked()
{
    _edit->show();
}

void UserPage::on_btnHistory_clicked()
{
    _hist->show();
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
    // ui->btnStart->setText("Waiting for players...");

    ui->infoLbl->setFont(_font);
    ui->infoLbl->setText("Waiting for players...");
}

void UserPage::onGameReady(const QJsonObject &gameInfo)
{
    QJsonArray players = gameInfo["players"].toArray();

    QMessageBox::information(this,
                             "Game Ready!",
                             QString("Enough players joined. Starting game now."));

    // Instantiate GamePage
    // Pass: player info, opponent, etc.
    GamePage *gp = new GamePage(_controller,_player, players, this);
    connect(gp, &QDialog::finished,this, &UserPage::show);
    connect(gp, &QDialog::finished,this, [this](int){
        ui->btnStart->setEnabled(true);
        ui->infoLbl->clear();
    });
    gp->setAttribute(Qt::WA_DeleteOnClose);
    gp->show();
    this->hide();
}

