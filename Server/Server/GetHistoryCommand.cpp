#include "GetHistoryCommand.h"

GetHistoryCommand::GetHistoryCommand(QObject *parent)
    : QObject{parent}
{}

void GetHistoryCommand::Execute(const QJsonObject &payload)
{
    QJsonObject err;
    err["cmd"]   = "GET_HISTORY_SUCCESS";
    for(int i=0;!hist.empty();i++){
        History itr = hist.front();
        err["date"+QString::number(i)] = itr.date.toString();
        err["date"+QString::number(i)] = itr.opponent;
        err["date"+QString::number(i)] = itr.result;
        err["date"+QString::number(i)] = itr.rounds[1]+"-"+itr.rounds[2]+"-"+itr.rounds[3];
    }
    emit ExecuteSuccessfully(err);
}

bool GetHistoryCommand::Validate(const QJsonObject &payload)
{
    QString username = payload.value("username").toString();
    auto itr = data->getData()->find(username);
    if(itr == data->getData()->end())
        return false;
    else{
        hist = itr->getHistory();
        if(hist.empty())
            return false;
        else
            return true;
    }
}

void GetHistoryCommand::operator()(const QJsonObject &obj)
{
    if(Validate(obj)){
        Execute(obj);
    }
    else{
        QJsonObject err;
        err["cmd"]   = "GET_HISTORY_FAILED";
        err["error"] = "No History Available";
        emit ValidationFailed(err);
    }
}

void GetHistoryCommand::operator[](Users *u)
{
    data = u;
}
