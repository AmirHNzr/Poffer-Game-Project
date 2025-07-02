#include "ForgotCommand.h"



void ForgotCommand::Execute(const QJsonObject &payload)
{
    QJsonObject err;
    err["cmd"]   = "LOGIN_SUCCESS";
    err["error"] = "Welcome " + payload.value("username").toString() +"!" ;
    emit ExecuteSuccessfully(err);
}

bool ForgotCommand::Validate(const QJsonObject &payload)
{
    QString username = payload.value("username").toString();
    QString phone = payload.value("phone").toString();
    if(data->getData()->find(username) == data->getData()->end()){
        qDebug() << "in forgot user failed";
        return false;

    }
    qDebug() << data->getData()->find(username).value().getPhoneNum();
    qDebug() << phone;

    return data->getData()->find(username).value().getPhoneNum() == phone;

}

void ForgotCommand::operator()(const QJsonObject& obj)
{
    qDebug() << "in forgot";
    if(Validate(obj)){
        qDebug() << "in forgot phone valid";
        Execute(obj);
    }
    else{
        qDebug() << "in forgot phone invalid";

        QJsonObject err;
        err["cmd"]   = "LOGIN_FAILED";
        err["error"] = "Incorrect username or phone number";
        emit ValidationFailed(err);
    }
}

void ForgotCommand::operator[](Users *u)
{
    data = u;
}

