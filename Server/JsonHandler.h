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
#include"ForgotCommand.h"
#include"GameManager.h"
#include"Users.h"


class JsonHandler : public QObject
{
    Q_OBJECT
public:
    explicit JsonHandler(GameManager*,Users* u=nullptr,QObject* parent = nullptr);

    void Commands(const QJsonObject&,QTcpSocket* _current=nullptr);
    QJsonObject BytesToJson(const QByteArray&);

    Users *getDataBase() const;

private:
    QJsonParseError _parseError;
    QByteArray _accumulator;

    LoginCommand _loginHandler;
    RegisterCommand _registerHandler;
    GetHistoryCommand _historyHandler;
    EditCommand _editHandler;
    ForgotCommand _forgotHandler;
    GameManager* gm;

    Users* dataBase;
signals:
    void ValidationFailed(const QJsonObject& errorPayload);
    void ValidationSuccessful(const QJsonObject& successPayload);




};

#endif // JSONHANDLER_H
