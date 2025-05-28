#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <QTcpSocket>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
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
signals:
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState);
    void errorOccurred(QAbstractSocket::SocketError);

    private slots:
    void socket_stateChanged(QAbstractSocket::SocketState state);


private:
    QTcpSocket _socket;
    QString ip;
    int port;
};

#endif // USERCONTROLLER_H
