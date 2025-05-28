#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupConnection();
    SetupLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupConnection()
{
    connect(&_controller, &UserController::connected, this, &MainWindow::UserConnected);
    connect(&_controller, &UserController::disconnected, this, &MainWindow::UserDisconnected);
    connect(&_controller, &UserController::stateChanged, this, &MainWindow::UserStateChanged);
    connect(&_controller, &UserController::errorOccurred, this, &MainWindow::UserErrorOccurred);
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
    if (_controller.isConnected()) {
        _controller.disconnect();
    } else {

        _controller.EstablishConnection(ip,port);
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



