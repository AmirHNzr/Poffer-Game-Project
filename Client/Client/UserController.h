#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <QTcpSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QCryptographicHash>
#include"JsonHandler.h"

class UserController : public QObject
{
    Q_OBJECT
public:
    explicit UserController(QObject *parent = nullptr);
    void EstablishConnection(QString,int);
    bool isConnected();


    void disconnect();
    QAbstractSocket::SocketState state();

    // For sending information over network from client to server
    void sendJson(const QJsonObject& obj);
public slots:
    // Registeration slot to send data to server
    void registerUser(const QString &firstname,const QString &lastname,
                      const QString &number,const QString &email,
                      const QString &username, const QString &password);

    void loginUser(const QString &username, const QString &password);

    void GetHistory(const QString &username);

    void EditProfile(const QString&);
signals:
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState);
    void errorOccurred(QAbstractSocket::SocketError);
    void jsonReceived(QJsonDocument);
    void OnSuccessfulLogin();

private slots:
    void socket_stateChanged(QAbstractSocket::SocketState state);
    void OnReadyRead();


private:
    QTcpSocket _socket;
    QByteArray _buffer;
    JsonHandler _jsonHandler;
    QString ip;
    int port;
};

#endif // USERCONTROLLER_H
