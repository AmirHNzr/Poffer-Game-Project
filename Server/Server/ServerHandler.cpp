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
    emit NewConnection();
}

void ServerHandler::OnClientDC(){

}

QHash<qintptr, QTcpSocket *> ServerHandler::getClients() const
{
    return clients;
}


bool ServerHandler::getIsOn() const
{
    return isOn;
}
