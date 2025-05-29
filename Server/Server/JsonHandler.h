#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include<QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include"LoginCommand.h"
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
    Users* dataBase;
signals:
    void LogValidationFailed(const QJsonObject& errorPayload);




};

#endif // JSONHANDLER_H
