#ifndef USERS_H
#define USERS_H


#include"User.h"
class Users
{
public:
    Users();
private:
    std::unordered_map<int,User> data;

};

#endif // USERS_H
