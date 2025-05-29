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

QHash<QString, User> *Users::getData()
{
    return &data;
}

