#ifndef ICOMMAND_H
#define ICOMMAND_H

#include<QJsonDocument>
#include<QJsonObject>
#include <QJsonParseError>

class ICommand
{
public:
    virtual ~ICommand(){}
    virtual QJsonObject Execute(const QJsonObject& payload) = 0;
    virtual bool Validate(const QJsonObject& payload) = 0;
};

#endif // ICOMMAND_H
