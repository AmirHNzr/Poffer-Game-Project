#ifndef LOGINCOMMAND_H
#define LOGINCOMMAND_H

#include"ICommand.h"

class LoginCommand : ICommand
{
public:
    LoginCommand();
    QJsonObject Execute(const QJsonObject& payload) override;
    bool Validate(const QJsonObject& payload) override;

    void operator()(const QJsonObject& obj) override;
    void operator[](Users* u) override;
private:
    QJsonObject obj;
    Users* data;

};

#endif // LOGINCOMMAND_H
