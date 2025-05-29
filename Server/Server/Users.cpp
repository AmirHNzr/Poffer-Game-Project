#include "Users.h"

Users::Users():data{nullptr} {}

Users::~Users(){
    delete data;
}

QHash<QString, User> *Users::getData() const
{
    return data;
}

