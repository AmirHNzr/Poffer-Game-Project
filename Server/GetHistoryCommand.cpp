#include "GetHistoryCommand.h"

GetHistoryCommand::GetHistoryCommand(QObject *parent)
    : QObject{parent}
{}

void GetHistoryCommand::Execute(const QJsonObject &payload)
{
    qDebug() << "[GET HISTORY] {EXECUTION}";
    int i=1;
    int count = 0;
    QJsonArray historyArray;
    for(auto& itr: hist){
        if(count != 3){
        QJsonObject oneEntry;
        oneEntry["date"]     = itr.date.toString(Qt::ISODate);
        oneEntry["opponent"] = itr.opponent;
        oneEntry["result"]   = itr.result;
        for(auto& r:itr.rounds){
            oneEntry[QString("round%1").arg(i++)] = r;
        }
        historyArray.append(oneEntry);
        count++;
        }
        i=1;
    }
    qDebug() << "[GET HISTORY] {EXECUTION} \\History Array CREATED/";

    QJsonObject err;
    err["cmd"]     = "GET_HISTORY_SUCCESS";
    err["history"] = historyArray;
    err["error"] = "History loaded successfully!";

    qDebug() << "[GET HISTORY] {EXECUTION} full JSON";
    qDebug() << err;
    qDebug() << "''''''''''''''''''''''''''''''''";



    emit ExecuteSuccessfully(err);
}

bool GetHistoryCommand::Validate(const QJsonObject &payload)
{
    qDebug() << "[GET HISTORY] {VALIDATION}";
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
