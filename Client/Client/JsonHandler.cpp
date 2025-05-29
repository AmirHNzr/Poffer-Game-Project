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
    else if(cmd == "REGISTER_FAILED")
        QMessageBox::warning(nullptr,"Register error",obj["error"].toString());
    else if(cmd == "REGISTER_SUCCESS")
        QMessageBox::warning(nullptr,"Register Complete",obj["error"].toString());


}
