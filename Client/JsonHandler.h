#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonArray>
#include"History.h"
#include<QDebug>
class JsonHandler : public QObject
{
    Q_OBJECT
public:
    explicit JsonHandler(QObject *parent = nullptr);

signals:
    void OnSuccessfulLogin();
    void editPermission();
    void GameReady(const QJsonObject &gameInfo);
    void HistReady();

public slots:
    void JsonReceived(QJsonDocument);
private:
};

#endif // JSONHANDLER_H
