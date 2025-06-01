#include "Users.h"

Users::Users(){}

Users::~Users(){}

bool Users::AddUser(const User& u)
{
    if(data.contains(u.getUserName()))
        return false;
    this->data.emplace(u.getUserName(),u);
    return true;
}

bool Users::EditUser(QString username,User newData)
{
    auto itr = data.find(newData.getUserName());
    if(itr == data.end()){
        data.insert(newData.getUserName(),newData);
        data.remove(username);
        return true;
    }
    else{

        return false;
    }
}

QHash<QString, User> *Users::getData()
{
    return &data;
}

