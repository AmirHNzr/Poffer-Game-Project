#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include<QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include"LoginCommand.h"
#include"Users.h"


class JsonHandler
{
public:
    JsonHandler();

    void Commands(const QJsonObject&);
    QJsonObject BytesToJson(const QByteArray&);

private:
    QJsonParseError _parseError;
    LoginCommand _loginHandler;
    Users* dataBase;



};

#endif // JSONHANDLER_H
