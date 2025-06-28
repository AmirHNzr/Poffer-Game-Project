#include "GetHistoryCommand.h"

GetHistoryCommand::GetHistoryCommand(QObject *parent)
    : QObject{parent}
{}

void GetHistoryCommand::Execute(const QJsonObject &payload)
{
    QJsonArray historyArray;
    for(auto& itr: hist){
        QJsonObject oneEntry;
        oneEntry["date"]     = itr.date.toString(Qt::ISODate);
        oneEntry["opponent"] = itr.opponent;
        oneEntry["result"]   = itr.result;
        oneEntry["rounds"]   = itr.rounds[1] + "-" + itr.rounds[2] + "-" + itr.rounds[3];
        historyArray.append(oneEntry);
    }
    QJsonObject err;
    err["cmd"]     = "GET_HISTORY_SUCCESS";
    err["history"] = historyArray;
    err["error"] = "History loaded successfully!";
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
