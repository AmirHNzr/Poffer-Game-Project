#include "JsonHandler.h"

JsonHandler::JsonHandler(QObject *parent)
    : QObject{parent}
{}

void JsonHandler::JsonReceived(QJsonDocument doc)
{
    hist.clear();
    QJsonObject obj = doc.object();
    auto cmd = obj["cmd"];

    if(cmd == "LOGIN_FAILED")
        QMessageBox::warning(nullptr,"Login error",obj["error"].toString());
    else if(cmd == "LOGIN_SUCCESS"){
        QMessageBox::information(nullptr,"Login Complete",obj["error"].toString());
        emit OnSuccessfulLogin();
    }

    else if(cmd == "REGISTER_FAILED")
        QMessageBox::warning(nullptr,"Register error",obj["error"].toString());
    else if(cmd == "REGISTER_SUCCESS")
        QMessageBox::information(nullptr,"Register Complete",obj["error"].toString());

    else if(cmd == "GET_HISTORY_FAILED")
        QMessageBox::warning(nullptr,"Register error",obj["error"].toString());
    else if(cmd == "GET_HISTORY_SUCCESS"){
        hist.clear();
        QMessageBox::information(nullptr,"Register Complete",obj["error"].toString());
        QJsonArray arr = obj["history"].toArray();
        qDebug() << "QJSON ARRAY:";
        qDebug() << arr;
        qDebug() << "'''''''''''''''''''''''";
        for (int i = 0; i < arr.size(); ++i) {
            qDebug() << "loop:" << i;
            QJsonObject e = arr[i].toObject();
            QDateTime    d     = QDateTime::fromString(e["date"].toString(), Qt::ISODate);
            QString  opp   = e["opponent"].toString();
            QString  res   = e["result"].toString();
            int count=1;
            QString rounds[3] = {e["round1"].toString(), e["round2"].toString(), e["round3"].toString()};
            hist.push_back(History(d,opp,res,rounds));
            qDebug() << "HISTORY PUSHED";
        }

    }

    else if(cmd == "EDIT_FAILED"){
        QMessageBox::warning(nullptr,"Edit error",obj["error"].toString());
        }
    else if(cmd == "EDIT_SUCCESS"){
        QMessageBox::information(nullptr,"Edit Complete",obj["error"].toString());
            emit editPermission();}

    else if(cmd =="GAME_READY"){
            emit GameReady(obj);
        }

}


