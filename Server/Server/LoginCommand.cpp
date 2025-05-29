#include "LoginCommand.h"


LoginCommand::LoginCommand()
{}

QJsonObject LoginCommand::Execute(const QJsonObject &payload)
{

}

bool LoginCommand::Validate(const QJsonObject &payload)
{

}

void LoginCommand::operator()(const QJsonObject& obj)
{
    this->obj = obj;

}
