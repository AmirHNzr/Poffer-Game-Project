#include "JsonHandler.h"

JsonHandler::JsonHandler(GameManager* gm,Users* u,QObject* parent)
    : QObject(parent),gm{gm},dataBase{u}
{
    connect(&_registerHandler,&RegisterCommand::ValidationFailed,this,&JsonHandler::ValidationFailed);
    connect(&_registerHandler,&RegisterCommand::ExecuteSuccessfully,this,&JsonHandler::ValidationSuccessful);

    connect(&_loginHandler,&LoginCommand::ExecuteSuccessfully,this,&JsonHandler::ValidationSuccessful);
    connect(&_loginHandler,&LoginCommand::ValidationFailed,this,&JsonHandler::ValidationFailed);

    connect(&_historyHandler,&GetHistoryCommand::ExecuteSuccessfully,this,&JsonHandler::ValidationSuccessful);
    connect(&_historyHandler,&GetHistoryCommand::ValidationFailed,this,&JsonHandler::ValidationFailed);

    connect(&_editHandler,&EditCommand::ExecuteSuccessfully,this,&JsonHandler::ValidationSuccessful);
    connect(&_editHandler,&EditCommand::ValidationFailed,this,&JsonHandler::ValidationFailed);
}


void JsonHandler::Commands(const QJsonObject& obj,QTcpSocket* _current)
{
    auto cmd = obj.value("cmd").toString().toUpper();

    if(cmd == "LOGIN"){
        //Loading data
        _loginHandler[dataBase];
        //Processing command
        _loginHandler(obj);
    }
    else if(cmd == "REGISTER"){
        _registerHandler[dataBase];
        _registerHandler(obj);
    }
    else if(cmd == "GET_HISTORY"){
        _historyHandler[dataBase];
        _historyHandler(obj);
    }
    else if(cmd == "EDIT_PROFILE"){
        _editHandler[dataBase];
        _editHandler(obj);
    }
    else if(cmd == "JOIN_QUEUE"){
        gm->enqueuePlayer(obj.value("username").toString(),_current);
    }

}

// QJsonObject JsonHandler::BytesToJson(const QByteArray& bytes)
// {
//     // Making a json doc out of bytes
//     QJsonDocument doc = QJsonDocument::fromJson(bytes,&_parseError);
//     // Checking whether or not we screwed the bytes
//     if(_parseError.error != QJsonParseError::NoError){
//         qDebug() << "[JsonHandler] parseError at offset" << _parseError.offset
//                  << ":" << _parseError.errorString();
//         QMessageBox::warning(nullptr,"Json Parsing Error","Couldn't resolve a file out of bytes");
//         return QJsonObject{};
//     }
//     return doc.object();
// }

QJsonObject JsonHandler::BytesToJson(const QByteArray &bytes)
{
    // 1) Append new data to our accumulator
    _accumulator.append(bytes);

    // 2) Look for the first delimiter (‘\n’). If none yet, we cannot parse a full JSON.
    int newlineIndex = _accumulator.indexOf('\n');
    if (newlineIndex < 0) {
        // No full message yet; wait for more bytes
        return QJsonObject{};
    }

    // 3) Extract everything up to (but not including) the '\n'
    QByteArray oneMessage = _accumulator.left(newlineIndex).trimmed();

    // 4) Remove that chunk + the delimiter from the accumulator
    //    so that whatever remains (possibly the start of next JSON) stays in _accumulator.
    _accumulator.remove(0, newlineIndex + 1);

    // 5) Now try parsing oneMessage as JSON
    QJsonDocument doc = QJsonDocument::fromJson(oneMessage, &_parseError);
    if (_parseError.error != QJsonParseError::NoError) {
        qDebug() << "[JsonHandler] parseError at offset" << _parseError.offset
                 << ":" << _parseError.errorString();
        QMessageBox::warning(nullptr,
                             "Json Parsing Error",
                             "Couldn't resolve a file out of bytes:\n" +
                                 oneMessage);
        return QJsonObject{};
    }

    if (!doc.isObject()) {
        qDebug() << "[JsonHandler] parsed JSON is not an object:" << oneMessage;
        return QJsonObject{};
    }

    return doc.object();
}

Users *JsonHandler::getDataBase() const
{
    return dataBase;
}
