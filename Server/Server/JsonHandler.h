#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include<QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include"LoginCommand.h"
#include"RegisterCommand.h"
#include"Users.h"


class JsonHandler : public QObject
{
    Q_OBJECT
public:
    explicit JsonHandler(QObject* parent = nullptr);

    void Commands(const QJsonObject&);
    QJsonObject BytesToJson(const QByteArray&);

private:
    QJsonParseError _parseError;
    LoginCommand _loginHandler;
    RegisterCommand _registerHandler;
    Users* dataBase;
signals:
    void LogValidationFailed(const QJsonObject& errorPayload);
    void ValidRegister(const QJsonObject& successPayload);




};

#endif // JSONHANDLER_H
