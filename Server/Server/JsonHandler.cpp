#include "JsonHandler.h"

JsonHandler::JsonHandler(QObject* parent)
    : QObject(parent),dataBase{new Users()}
{
    connect(&_loginHandler,&LoginCommand::ValidationFailed,this,&JsonHandler::ValidationFailed);
    connect(&_registerHandler,&RegisterCommand::ValidationFailed,this,&JsonHandler::ValidationFailed);
    connect(&_registerHandler,&RegisterCommand::ExecuteSuccessfully,this,&JsonHandler::ValidationSuccessful);
    connect(&_loginHandler,&LoginCommand::ExecuteSuccessfully,this,&JsonHandler::ValidationSuccessful);

}


void JsonHandler::Commands(const QJsonObject& obj)
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


}

QJsonObject JsonHandler::BytesToJson(const QByteArray& bytes)
{
    // Making a json doc out of bytes
    QJsonDocument doc = QJsonDocument::fromJson(bytes,&_parseError);
    // Checking whether or not we screwed the bytes
    if(_parseError.error != QJsonParseError::NoError){
        QMessageBox::warning(nullptr,"Json Parsing Error","Couldn't resolve a file out of bytes");
        return QJsonObject{};
    }
    return doc.object();
}
