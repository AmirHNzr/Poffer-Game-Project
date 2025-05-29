#ifndef ICOMMAND_H
#define ICOMMAND_H

#include<QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>
#include"Users.h"
class ICommand
{
public:
    virtual ~ICommand(){}
    virtual QJsonObject Execute(const QJsonObject& payload) = 0;
    virtual bool Validate(const QJsonObject& payload) = 0;
    //Processing Client Command
    virtual void operator()(const QJsonObject&) = 0;
    //Loading All Clients Data
    virtual void operator[](Users*) = 0;
};

#endif // ICOMMAND_H
