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
public slots:
    void JsonReceived(QJsonDocument);
};

#endif // JSONHANDLER_H
