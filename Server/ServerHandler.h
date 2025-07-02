#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>
#include"JsonHandler.h"
#include"GameManager.h"
#include"GameSession.h"
#include"Users.h"


class ServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServerHandler(QObject *parent = nullptr,int port=12345);

    bool getIsOn() const;

    QHash<qintptr, QTcpSocket *> getClients() const;

    QJsonObject getReceivedData() const;

    Users *users();

    void ReadFile();

signals:
    void NewConnection();
    void NewDC();
    void NewDataSent();

private slots:
    void OnNewConnection();
    void OnClientDC();
    void OnReadyRead();
    void OnSendError(const QJsonObject& errorPayload);
private:
    QTcpServer* _server;
    QTcpSocket* _currentSocket = nullptr;
    QHash<qintptr, QTcpSocket*> clients;
    bool isOn;
    JsonHandler* _jsonHandler;
    QJsonObject receivedData;
    GameManager* _gm;
    GameSession* _session;
    Users* _users;
};

#endif // SERVERHANDLER_H
