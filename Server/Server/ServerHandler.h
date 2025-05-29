#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>
#include"JsonHandler.h"

class ServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServerHandler(QObject *parent = nullptr,int port=12345);

    bool getIsOn() const;

    QHash<qintptr, QTcpSocket *> getClients() const;

    QJsonObject getReceivedData() const;

signals:
    void NewConnection();
    void NewDC();

private slots:
    void OnNewConnection();
    void OnClientDC();
    void OnReadyRead();
private:
    QTcpServer* _server;
    QHash<qintptr, QTcpSocket*> clients;
    bool isOn;
    JsonHandler _jsonHandler;
    QJsonObject receivedData;

};

#endif // SERVERHANDLER_H
