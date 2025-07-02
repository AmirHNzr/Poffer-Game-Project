#include "RegisterCommand.h"

RegisterCommand::RegisterCommand(QObject *parent)
    : QObject{parent}
{}

void RegisterCommand::Execute(const QJsonObject &payload)
{
    QString fn = payload.value("firstname").toString();
    QString ln = payload.value("lastname").toString();
    QString num = payload.value("number").toString();
    QString em = payload.value("email").toString();
    QString un = payload.value("username").toString();
    QString p = payload.value("password").toString();

    bool isAdded = data->AddUser(User(fn,ln,num,em,un,p));
    qDebug() << "Before Writing file>>>\n";
    data->WriteFile(User(fn,ln,num,em,un,p));
    if(isAdded){
        QJsonObject err;
        err["cmd"]   = "REGISTER_SUCCESS";
        err["error"] = "You Registered Successfully";
        emit ExecuteSuccessfully(err);
    }
    else{
        QJsonObject err;
        err["cmd"]   = "REGISTER_FAILED";
        err["error"] = "Username is taken!";
        emit ValidationFailed(err);
    }

}

bool RegisterCommand::Validate(const QJsonObject &payload)
{
    QString username = payload.value("username").toString();
    if(data->getData()->find(username) != data->getData()->end())
        return false;
    return true;

}

void RegisterCommand::operator()(const QJsonObject& obj)
{
    if(Validate(obj)){
        Execute(obj);
    }
    else{
        QJsonObject err;
        err["cmd"]   = "REGISTER_FAILED";
        err["error"] = "Username has been taken!";
        emit ValidationFailed(err);
    }
}

void RegisterCommand::operator[](Users *u)
{
    data = u;
}
