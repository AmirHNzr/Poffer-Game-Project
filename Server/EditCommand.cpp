#include "EditCommand.h"

EditCommand::EditCommand(QObject *parent)
    : QObject{parent}
{}

void EditCommand::Execute(const QJsonObject &payload)
{
    QString fn = payload.value("firstname").toString();
    QString ln = payload.value("lastname").toString();
    QString num = payload.value("number").toString();
    QString em = payload.value("email").toString();
    QString un = payload.value("second_username").toString();
    QString p = payload.value("password").toString();

    bool isAdded = data->EditUser(payload.value("first_username").toString(),User(fn,ln,num,em,un,p));
    if(isAdded){
        QJsonObject err;
        err["cmd"]   = "EDIT_SUCCESS";
        err["error"] = "Edited Successfully";
        emit ExecuteSuccessfully(err);
    }
    else{
        QJsonObject err;
        err["cmd"]   = "EDIT_FAILED";
        err["error"] = "Username is taken!";
        emit ValidationFailed(err);
    }

}

bool EditCommand::Validate(const QJsonObject &payload)
{
    QString fisr_username = payload.value("first_username").toString();
    QString sec_username = payload.value("second_username").toString();
    if(fisr_username == sec_username)
        return true;
    if(data->getData()->find(sec_username) == data->getData()->end())
        return true;
    return false;

}

void EditCommand::operator()(const QJsonObject& obj)
{

    if(Validate(obj)){
        Execute(obj);
    }
    else{
        QJsonObject err;
        err["cmd"]   = "EDIT_FAILED";
        err["error"] = "Username has been taken!";
        emit ValidationFailed(err);
    }
}

void EditCommand::operator[](Users *u)
{
    data = u;
}
