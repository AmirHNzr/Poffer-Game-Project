#include "ServerHandler.h"

ServerHandler::ServerHandler(QObject *parent,int port)
    : QObject{parent}
{
    _users = new Users();
    _server = new QTcpServer(this);
    _gm = new GameManager();
    _jsonHandler = new JsonHandler(_gm,_users);
    _session = new GameSession(_gm,_users);
    clients.reserve(16);



    //To handle new connections
    connect(_server,&QTcpServer::newConnection,this,&ServerHandler::OnNewConnection);

    //If login failed, send an error json
    connect(_jsonHandler,&JsonHandler::ValidationFailed,this,&ServerHandler::OnSendError);

    // Handling when registeration succeed
    connect(_jsonHandler,&JsonHandler::ValidationSuccessful,this,&ServerHandler::OnSendError);


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
    qDebug() << "[ServerHandler]   Accepted new client from"
             << sock->peerAddress().toString()
             << ":"
             << sock->peerPort();
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

// void ServerHandler::OnReadyRead()
// {
//     //Who is sending
//     QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
//     if (!sock) return;

//     _currentSocket = sock;

//     QByteArray raw = sock->readAll();
//     qDebug() << "[ServerHandler] raw bytes received:" << raw;

//     receivedData = _jsonHandler->BytesToJson(raw);
//     _jsonHandler->Commands(receivedData,_currentSocket);
//     qDebug() << "[ServerHandler] about to call GameSession with:" << receivedData;
//     qDebug() << "[ServerHandler] socket" << sock->socketDescriptor();
//     (*_session)(receivedData);

// }

void ServerHandler::OnReadyRead()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    if (!sock) return;

    _currentSocket = sock;

    // Process all available complete lines
    while (sock->canReadLine()) {
        QByteArray line = sock->readLine().trimmed();
        qDebug() << "[ServerHandler] received line:" << line;

        if (line.isEmpty()) continue;

        // Parse JSON from this line
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(line, &error);

        if (error.error != QJsonParseError::NoError) {
            qWarning() << "[ServerHandler] JSON parse error:" << error.errorString()
            << "for line:" << line;
            continue;
        }

        if (!doc.isObject()) {
            qWarning() << "[ServerHandler] JSON is not an object:" << line;
            continue;
        }

        QJsonObject receivedData = doc.object();
        qDebug() << "[ServerHandler] processed JSON:" << receivedData;

        // Process each JSON message
        _jsonHandler->Commands(receivedData, _currentSocket);

        qDebug() << "[ServerHandler] about to call GameSession with:" << receivedData;
        qDebug() << "[ServerHandler] socket" << sock->socketDescriptor();
        //if(receivedData["cmd"] == "PICKED") return;
        (*_session)(receivedData);
    }
}


void ServerHandler::OnSendError(const QJsonObject &errorPayload)
{
    QJsonDocument doc(errorPayload);
    QByteArray bytes = doc.toJson(QJsonDocument::Compact) + "\n";


    if (!_currentSocket)
        return;
    _currentSocket->write(bytes);
    _currentSocket->flush();
    emit NewDataSent();
}

Users *ServerHandler::users()
{
    return _users;
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
