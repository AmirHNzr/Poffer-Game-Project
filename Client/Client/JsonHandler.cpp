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
}
