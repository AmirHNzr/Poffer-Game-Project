#include "JsonHandler.h"

JsonHandler::JsonHandler(QObject *parent)
    : QObject{parent}
{}

void JsonHandler::JsonReceived(QJsonDocument doc)
{
    QJsonObject obj = doc.object();
    auto cmd = obj["cmd"];
    if(cmd == "LOGIN_FAILED")
        QMessageBox::warning(nullptr,"Login error",obj["error"].toString());
    else if(cmd == "LOGIN_SUCCESS")
        QMessageBox::information(nullptr,"Login Complete",obj["error"].toString());
    else if(cmd == "REGISTER_FAILED")
        QMessageBox::warning(nullptr,"Register error",obj["error"].toString());
    else if(cmd == "REGISTER_SUCCESS")
        QMessageBox::information(nullptr,"Register Complete",obj["error"].toString());


}
