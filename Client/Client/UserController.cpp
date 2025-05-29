#include "UserController.h"

UserController::UserController(QObject *parent)
    : QObject{parent}
{

    connect(&_socket,&QTcpSocket::connected,this,&UserController::connected);
    connect(&_socket,&QTcpSocket::disconnected,this,&UserController::disconnected);
    connect(&_socket,&QTcpSocket::stateChanged,this,&UserController::socket_stateChanged);
    connect(&_socket,&QTcpSocket::errorOccurred,this,&UserController::errorOccurred);
    connect(&_socket, &QTcpSocket::readyRead,this,&UserController::OnReadyRead);
    connect(this,&UserController::jsonReceived,&_jsonHandler, &JsonHandler::JsonReceived);


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
        //Ciphering the password (note: works only on QByteArray)
        QByteArray passByte = password.toUtf8();
        QByteArray hashBytes = QCryptographicHash::hash(passByte,QCryptographicHash::Sha256);
        QString passBase64 = hashBytes.toBase64();

        //Creating a json file to make clear the command and files
        QJsonObject req {
            { "cmd",      "REGISTER"        },
            { "firstname",firstname         },
            { "lastname", lastname          },
            { "number",   number            },
            { "email",    email             },
            { "username", username          },
            { "password", passBase64        }
                        };
        sendJson(req);
    }
    else{
        QMessageBox::warning(nullptr,"Connection Error","You are not connected to the server");
    }
}

void UserController::loginUser(const QString &username, const QString &password)
{
    if(_socket.isOpen()){
        //Ciphering the password (note: works only on QByteArray)
        QByteArray passByte = password.toUtf8();
        QByteArray hashBytes = QCryptographicHash::hash(passByte,QCryptographicHash::Sha256);
        QString passBase64 = hashBytes.toBase64();

        //Creating a json file to make clear the command and files
        QJsonObject req {
            { "cmd",      "LOGIN"        },
            { "username", username          },
            { "password", passBase64        }
        };
        sendJson(req);
    }
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

void UserController::OnReadyRead()
{
    _buffer += _socket.readAll();

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(_buffer, &err);

    if (err.error == QJsonParseError::NoError && doc.isObject()) {
        emit jsonReceived(doc);
        _buffer.clear();
    }
}
