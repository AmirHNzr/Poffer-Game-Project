#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>


class ServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServerHandler(QObject *parent = nullptr,int port=12345);

    bool getIsOn() const;

    QHash<qintptr, QTcpSocket *> getClients() const;

signals:
    void NewConnection();

private slots:
    void OnNewConnection();
    void OnClientDC();
private:
    QTcpServer* _server;
    QHash<qintptr, QTcpSocket*> clients;
    bool isOn;

};

#endif // SERVERHANDLER_H
