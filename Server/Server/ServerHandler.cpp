#include "ServerHandler.h"

ServerHandler::ServerHandler(QObject *parent,int port)
    : QObject{parent}
{
    _server = new QTcpServer(this);
    clients.reserve(16);


    //To handle new connections
    connect(_server,&QTcpServer::newConnection,this,&ServerHandler::OnNewConnection);

    //Start listening and reserving port 12345 for any client
    isOn = _server->listen(QHostAddress::AnyIPv4,port);

    //Handling close ports
    if(!isOn)
        QMessageBox::warning(nullptr,"Port issue","It seems this port is reserved");
}

void ServerHandler::OnNewConnection()
{
    // Returns pointer to the incoming socket
    auto sock = _server->nextPendingConnection();
    if(!sock) return;

    // Returns an ID or descriptor for this socket (if not listening -1)
    auto id = sock->socketDescriptor();

    clients.insert(id,sock);

    connect(sock,&QTcpSocket::disconnected,this,&ServerHandler::OnClientDC);
    //Reading data from socket
    connect(sock, &QTcpSocket::readyRead,this, &ServerHandler::OnReadyRead);
    emit NewConnection();


}

void ServerHandler::OnClientDC(){

    //Sender is the one that disconnected so we get its socket and match with the id in QHash to remove
    auto sock = qobject_cast<QTcpSocket*>(sender());
    disconnect(sock,&QTcpSocket::disconnected,this,&ServerHandler::OnClientDC);

    if (!sock) return;

    qintptr id = sock->socketDescriptor();
    clients.remove(id);
    sock->deleteLater();

    emit NewDC();
}

void ServerHandler::OnReadyRead()
{
    //Who is sending
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    if (!sock) return;

    QByteArray raw = sock->readAll();
    receivedData = _jsonHandler.BytesToJson(raw);
    emit NewData();
    return;

}

QJsonObject ServerHandler::getReceivedData() const
{
    return receivedData;
}

QHash<qintptr, QTcpSocket *> ServerHandler::getClients() const
{
    return clients;
}


bool ServerHandler::getIsOn() const
{
    return isOn;
}
