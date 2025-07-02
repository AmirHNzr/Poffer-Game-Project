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
        _svHandler->ReadFile();

        connect(_svHandler,&ServerHandler::NewConnection,this,&MainWindow::NewConnection);
        connect(_svHandler,&ServerHandler::NewDC,this,&MainWindow::NewDC);
        connect(_svHandler,&ServerHandler::NewDataSent,this,&MainWindow::NewDataSent);

        QString ip = findLocalIP();
        ui->ipLbl->setText("IP:"+ip);

    }
    else{
        disconnect(_svHandler,&ServerHandler::NewConnection,this,&MainWindow::NewConnection);
        disconnect(_svHandler,&ServerHandler::NewDC,this,&MainWindow::NewDC);
        _svHandler->disconnect();
        ui->btnStartServer->setText("Start Server");
        ui->spnConnection->setValue(0);

        _svHandler->deleteLater();
        _svHandler = nullptr;
        ui->ipLbl->clear();
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
void MainWindow::NewDataSent(){
    ui->teServer->append("a data sent to client\n");

}

QString MainWindow::findLocalIP() const
{
    //Iterate all interfaces
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces()) {
        if (iface.humanReadableName() != QStringLiteral("Wi-Fi")) continue;

        for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            QHostAddress ip = entry.ip();
            if (ip.protocol() == QAbstractSocket::IPv4Protocol && !ip.isLoopback())
                return ip.toString();
        }
    }
    return QStringLiteral("127.0.0.1");
}

