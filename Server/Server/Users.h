#ifndef USERS_H
#define USERS_H


#include"User.h"
class Users
{
public:
    Users();

    bool AddUser(User);
    bool EditUser(User);
private:
    //i am using Username as the key for each user in this map
    //std::unordered_map<QString,User> data;

};

#endif // USERS_H
