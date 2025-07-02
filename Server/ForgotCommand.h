#ifndef FORGOTCOMMAND_H
#define FORGOTCOMMAND_H

#include <QObject>

#include <QObject>
#include "ICommand.h"

class ForgotCommand : public QObject, public ICommand
{
    Q_OBJECT
public:
    explicit ForgotCommand(QObject* parent = nullptr)
        : QObject(parent){}

    void Execute(const QJsonObject& payload) override;
    bool Validate(const QJsonObject& payload) override;

    void operator()(const QJsonObject& obj) override;
    void operator[](Users* u) override;
private:
    QJsonObject obj;
    Users* data;
signals:
    void ValidationFailed(const QJsonObject& errorPayload);
    void ExecuteSuccessfully(const QJsonObject& successPayload);


};
#endif // FORGOTCOMMAND_H
