#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    _controller = new UserController(this);
    _player = new PlayerInfo(this);
    //Setting up user page/menu
    _userPage = new UserPage(_controller,_player,this);

    ui->setupUi(this);
    //Setting connection up
    SetupConnection();
    //Center the widgets
    SetupLayout();
    //Whenever user checks register the login disbales and vice verca
    SetupLogRegButton();
    //Setting up register connections
    SetupRegister();
    //Setting up login connections
    SetupLogin();
    //Checking email
    EmailRegex();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupConnection()
{
    connect(_controller, &UserController::connected, this, &MainWindow::UserConnected);
    connect(_controller, &UserController::disconnected, this, &MainWindow::UserDisconnected);
    connect(_controller, &UserController::stateChanged, this, &MainWindow::UserStateChanged);
    connect(_controller, &UserController::errorOccurred, this, &MainWindow::UserErrorOccurred);
    connect(_controller, &UserController::OnSuccessfulLogin, this, [&](){permission = true;
                                                                         hide();
                                                                         _userPage->open();});

    connect(_userPage,&QDialog::finished,this,&MainWindow::show);
}

void MainWindow::on_lnIP_textChanged(const QString &arg1)
{
    QHostAddress addr(arg1);
    if(arg1 == "..."){
        ui->btnConnect->setProperty("state", "");
    }
    else{
    bool valid = (addr.protocol() == QAbstractSocket::IPv4Protocol);

    // set an *int* or *bool* property rather than a string:
    ui->btnConnect->setProperty("state", valid ? 1 : 0);}

    // re-apply the style sheet
    auto st = ui->btnConnect->style();
    st->unpolish(ui->btnConnect);
    st->polish(ui->btnConnect);
    ui->btnConnect->update();
}


void MainWindow::on_btnConnect_clicked()
{
    auto ip = ui->lnIP->text();
    int port = ui->spnPort->value();
    if (_controller->isConnected()) {
        _controller->disconnect();
    } else {

        _controller->EstablishConnection(ip,port);
    }
}




void MainWindow::UserConnected()
{
    ui->teConnection->append("Connected\n==========");
    ui->btnConnect->setText("Disconnect");

}

void MainWindow::UserDisconnected()
{
    ui->teConnection->append("Disconnected\n==========");
    ui->btnConnect->setText("Connect");


}

void MainWindow::UserStateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString keyString = metaEnum.valueToKey(state);
    ui->teConnection->append(keyString + "\n");
}

void MainWindow::UserErrorOccurred(QAbstractSocket::SocketError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError >();
    QString keyString = metaEnum.valueToKey(error);
    ui->teConnection->append(keyString + "\n"); }




void MainWindow::SetupLayout()
{
    // 1) Grab the “Connection” page
    QWidget *page = ui->tabWidget->widget(0);

    // 2) Hide the old designer‐made wrapper widget
    if (auto oldWrapper = ui->formLayout_3->parentWidget()) {
        oldWrapper->hide();
    }

    // 3) Create our new container, set it Expanding
    auto *container = new QWidget(page);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 4) Build the inner two‐column layout
    auto *hbox = new QHBoxLayout(container);
    hbox->setContentsMargins(0,0,0,0);
    hbox->setSpacing(10);

    // Left: your groupBox
    ui->groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hbox->addWidget(ui->groupBox, 1);

    // Right: log + clear button
    auto *logContainer = new QWidget(container);
    logContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *vbox = new QVBoxLayout(logContainer);
    vbox->setContentsMargins(0,0,0,0);
    vbox->setSpacing(5);

    ui->teConnection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    vbox->addWidget(ui->teConnection, 1);
    vbox->addWidget(ui->pushButton,   0);

    logContainer->setLayout(vbox);
    hbox->addWidget(logContainer, 1);

    container->setLayout(hbox);

    // 5) Wrap that in a 3×3 grid so it’s always centered
    auto *outer = new QGridLayout(page);
    outer->setContentsMargins(0,0,0,0);
    outer->addWidget(container, 1, 1);
    outer->setRowStretch(0,1);
    outer->setRowStretch(1,0);
    outer->setRowStretch(2,1);
    outer->setColumnStretch(0,1);
    outer->setColumnStretch(1,0);
    outer->setColumnStretch(2,1);
}

void MainWindow::SetupLogRegButton()
{
    connect(ui->chbLog, &QCheckBox::toggled,this, [=](bool checked){
        if (checked)
            ui->chbReg->setChecked(false);
    });
    connect(ui->chbReg, &QCheckBox::toggled,this, [=](bool checked){
        if (checked)
            ui->chbLog->setChecked(false);
    });
    connect(ui->chbLog, &QCheckBox::toggled,
            ui->grbLog, &QGroupBox::setEnabled);
    connect(ui->chbReg, &QCheckBox::toggled,
            ui->grbReg, &QGroupBox::setEnabled);

    //There is a bug for password line edits that i couldn't find so fixed this way:
    connect(ui->chbReg, &QCheckBox::toggled,
            ui->lnPassword, &QWidget::setEnabled);
    connect(ui->chbLog, &QCheckBox::toggled,
            ui->lnPasswordLog, &QWidget::setEnabled);


}

void MainWindow::SetupRegister(){
    connect(ui->btnReg, &QPushButton::clicked, this, [=]() {
        QString fn = ui->lnFname->text();
        QString ln = ui->lnLname->text();
        QString num = ui->lnPhone->text();
        QString email = ui->lnEmail->text();
        QString user = ui->lnUsername->text();
        QString pass = ui->lnPassword->text();
        _controller->registerUser(fn,ln,num,email,user,pass);
    });
}

void MainWindow::SetupLogin(){
    connect(ui->btnLog, &QPushButton::clicked, this, [=]() {
        QString user = ui->lnUsernameLog->text();
        QString pass = ui->lnPasswordLog->text();
        _controller->loginUser(user,pass);
    });
}

void MainWindow::EmailRegex()
{
    connect(ui->lnEmail, &QLineEdit::editingFinished, this, [this]() {
        const QString text = ui->lnEmail->text();
        // A tight regex that enforces local@domain.tld, for example:
        static const QRegularExpression re(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
        if (!re.match(text).hasMatch()) {
            QMessageBox::warning(this,
                                 tr("Invalid Email"),
                                 tr("“%1” is not a valid email address.").arg(text));
            ui->lnEmail->clear();
            ui->lnEmail->setFocus();
        }
    });

}






