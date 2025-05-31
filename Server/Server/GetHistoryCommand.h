#ifndef GETHISTORYCOMMAND_H
#define GETHISTORYCOMMAND_H

#include"ICommand.h"
#include <QObject>
#include <QJsonArray>
class GetHistoryCommand : public QObject, public ICommand
{
    Q_OBJECT
public:
    explicit GetHistoryCommand(QObject *parent = nullptr);
    void Execute(const QJsonObject& payload);
    bool Validate(const QJsonObject& payload);
    //Processing Client Command
    void operator()(const QJsonObject&);
    //Loading All Clients Data
    void operator[](Users*);
private:
    QJsonObject obj;
    Users* data;
    std::queue<History> hist;
signals:
    void ValidationFailed(const QJsonObject& errorPayload);
    void ExecuteSuccessfully(const QJsonObject& successPayload);
};

#endif // GETHISTORYCOMMAND_H
