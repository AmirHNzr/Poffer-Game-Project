#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::connectionsCount = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _svHandler = nullptr;
    ui->lnNumConnect->append(QString::number(connectionsCount));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartServer_clicked()
{
    _svHandler = _svHandler == nullptr ? new ServerHandler(this,ui->spnPort->value()): _svHandler;
    connect(_svHandler,&ServerHandler::NewConnection,this,&MainWindow::NewConnection);
}

void MainWindow::NewConnection(){
    connectionsCount++;
    ui->lnNumConnect->append(QString::number(connectionsCount));
    ui->teServer->append("New connection added\n");
    ui->teServer->append(_svHandler->getClients().keyValueBegin()->second->localAddress().toString());

}
