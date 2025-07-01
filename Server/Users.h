#ifndef USERS_H
#define USERS_H

#include<QObject>
#include<QHash>
#include"User.h"
#include<fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
class Users
{
public:
    Users();
    ~Users();
    bool AddUser(const User&);
    bool EditUser(QString,User);
    void WriteFile(User u);
    void ReadFile();
    QHash<QString, User>* getData();
    void AddHistory(QString username, History hist);

private:
    //i am using Username as the key for each user in this map
    //std::unordered_map<QString,User> data;
    QHash<QString,User> data;


};

#endif // USERS_H
