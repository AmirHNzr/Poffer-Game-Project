#include "Users.h"

Users::Users(){}

Users::~Users(){}

QHash<QString, User> *Users::getData()
{
    return &data;
}

