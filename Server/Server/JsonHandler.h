#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include<QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include"LoginCommand.h"
#include"RegisterCommand.h"
#include"GetHistoryCommand.h"
#include"EditCommand.h"
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
    GetHistoryCommand _historyHandler;
    EditCommand _editHandler;
    Users* dataBase;
signals:
    void ValidationFailed(const QJsonObject& errorPayload);
    void ValidationSuccessful(const QJsonObject& successPayload);




};

#endif // JSONHANDLER_H
