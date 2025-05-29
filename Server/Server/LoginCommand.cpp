#include "LoginCommand.h"



QJsonObject LoginCommand::Execute(const QJsonObject &payload)
{
    return QJsonObject{payload};
}

bool LoginCommand::Validate(const QJsonObject &payload)
{
    QString username = payload.value("username").toString();
    QString pass = payload.value("password").toString();
    if(data->getData()->find(username) == data->getData()->end())
        return false;
    return data->getData()->find(username).value().getPassword() == pass;

}

void LoginCommand::operator()(const QJsonObject& obj)
{
    if(Validate(obj)){
        Execute(obj);
    }
    else{
        QJsonObject err;
        err["cmd"]   = "LOGIN_FAILED";
        err["error"] = "Incorrect username or password";
        emit ValidationFailed(err);
    }
}

void LoginCommand::operator[](Users *u)
{
    data = u;
}
