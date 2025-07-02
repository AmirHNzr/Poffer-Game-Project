#ifndef REGISTERCOMMAND_H
#define REGISTERCOMMAND_H

#include <QObject>
#include "ICommand.h"
class RegisterCommand : public QObject , public ICommand
{
    Q_OBJECT
public:
    explicit RegisterCommand(QObject *parent = nullptr);
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
#endif // REGISTERCOMMAND_H
