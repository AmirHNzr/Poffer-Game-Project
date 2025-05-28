#include "UserController.h"

UserController::UserController(QObject *parent)
    : QObject{parent}
{

    connect(&_socket,&QTcpSocket::connected,this,&UserController::connected);
    connect(&_socket,&QTcpSocket::disconnected,this,&UserController::disconnected);
    connect(&_socket,&QTcpSocket::stateChanged,this,&UserController::socket_stateChanged);
    connect(&_socket,&QTcpSocket::errorOccurred,this,&UserController::errorOccurred);



}

void UserController::EstablishConnection(QString ip, int port)
{
    if(_socket.isOpen()){
        if(this->ip == ip && this->port == port)
            return;
        _socket.close();
    }
    this->ip = ip;
    this->port = port;
    _socket.connectToHost(this->ip,this->port);

}

bool UserController::isConnected()
{
    return _socket.state() == QAbstractSocket::ConnectedState;
}

void UserController::disconnect()
{
    _socket.close();
}

QAbstractSocket::SocketState UserController::state()
{
    return _socket.state();
}

void UserController::sendJson(const QJsonObject &obj)
{
    auto bytes = QJsonDocument(obj).toJson(QJsonDocument::Compact) + "\n";
    _socket.write(bytes);

}

void UserController::registerUser(const QString &firstname,const QString &lastname,const QString &number,const QString &email,
                                  const QString &username, const QString &password)
{
    if(_socket.isOpen()){
    QJsonObject req {
        { "cmd",      "REGISTER"        },
        { "firstname",firstname         },
        { "lastname", lastname          },
        { "number",   number            },
        { "email",    email             },
        { "username", username          },
        { "password", password          }
    };
        sendJson(req);}
    else{
        QMessageBox::warning(nullptr,"Connection Error","You are not connected to the server");
    }
}

void UserController::socket_stateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState) {
        _socket.close();
    }
    emit stateChanged(state);
}
