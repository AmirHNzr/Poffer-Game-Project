#ifndef LOGINCOMMAND_H
#define LOGINCOMMAND_H

#include <QObject>
#include "ICommand.h"

class LoginCommand : public QObject, public ICommand
{
    Q_OBJECT
public:
    explicit LoginCommand(QObject* parent = nullptr)
        : QObject(parent){}

    QJsonObject Execute(const QJsonObject& payload) override;
    bool Validate(const QJsonObject& payload) override;

    void operator()(const QJsonObject& obj) override;
    void operator[](Users* u) override;
private:
    QJsonObject obj;
    Users* data;
signals:
    void ValidationFailed(const QJsonObject& errorPayload);

};

#endif // LOGINCOMMAND_H
