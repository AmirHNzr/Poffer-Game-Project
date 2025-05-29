#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::connectionsCount = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _svHandler = nullptr;
    ui->spnConnection->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStartServer_clicked()
{
    if(_svHandler == nullptr){
        _svHandler = new ServerHandler(this,ui->spnPort->value()) ;
        ui->btnStartServer->setText("Stop Server");

        connect(_svHandler,&ServerHandler::NewConnection,this,&MainWindow::NewConnection);
        connect(_svHandler,&ServerHandler::NewDC,this,&MainWindow::NewDC);


    }
    else{
        disconnect(_svHandler,&ServerHandler::NewConnection,this,&MainWindow::NewConnection);
        disconnect(_svHandler,&ServerHandler::NewDC,this,&MainWindow::NewDC);
        _svHandler->disconnect();
        ui->btnStartServer->setText("Start Server");
        ui->spnConnection->setValue(0);

        _svHandler->deleteLater();
        _svHandler = nullptr;
    }
}

void MainWindow::NewConnection(){
    connectionsCount++;
    ui->spnConnection->setValue(connectionsCount);
    ui->teServer->append("a connection added\n");

}
void MainWindow::NewDC(){
    connectionsCount--;
    ui->spnConnection->setValue(connectionsCount);
    ui->teServer->append("a connection removed\n");

}


