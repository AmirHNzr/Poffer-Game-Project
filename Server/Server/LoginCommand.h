#ifndef LOGINCOMMAND_H
#define LOGINCOMMAND_H

#include"ICommand.h"

class LoginCommand : ICommand
{
public:
    LoginCommand();
    QJsonObject Execute(const QJsonObject& payload) override;
    bool Validate(const QJsonObject& payload) override;

    void operator()(const QJsonObject& obj);
private:
    QJsonObject obj;

};

#endif // LOGINCOMMAND_H
