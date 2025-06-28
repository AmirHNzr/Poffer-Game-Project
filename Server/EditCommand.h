#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include <QObject>
#include"ICommand.h"

class EditCommand : public QObject, public ICommand
{
    Q_OBJECT
public:
    explicit EditCommand(QObject *parent = nullptr);
    void Execute(const QJsonObject& payload);
    bool Validate(const QJsonObject& payload);
    //Processing Client Command
    void operator()(const QJsonObject&);
    //Loading All Clients Data
    void operator[](Users*);
private:
    QJsonObject obj;
    Users* data;
signals:
    void ValidationFailed(const QJsonObject& errorPayload);
    void ExecuteSuccessfully(const QJsonObject& successPayload);
};

#endif // EDITCOMMAND_H
