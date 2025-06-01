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

bool Users::EditUser(QString oldUsername,User newData)
{
    auto itrOld = data.find(oldUsername);
    if (itrOld == data.end()) {
        return false;
    }

    // 2) If the client isn't changing the username, do an in-place update:
    QString newUsername = newData.getUserName();
    if (newUsername == oldUsername) {
        itrOld->setFirstName(newData.getFirstName());
        itrOld->setLastName(newData.getLastName());
        itrOld->setEMail(newData.getEMail());
        itrOld->setPassword(newData.getPassword());
        itrOld->setPhoneNum(newData.getPhoneNum());
        return true;
    }

    // 3) Now the user really wants to change their username:
    if (data.contains(newUsername)) {
        return false;
    }

    // 4) Build a copy carrying over the old history, then insert/erase:
    User updatedUser = newData;
    updatedUser.setHistory(itrOld->getHistory());

    data.insert(newUsername, updatedUser);
    data.remove(oldUsername);
    return true;
}

QHash<QString, User> *Users::getData()
{
    return &data;
}

