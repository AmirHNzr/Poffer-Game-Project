#include "User.h"


User::User(QString fName, QString lName,int num,
           QString mail,QString user, QString pass):firstName{fName},lastName{lName}
                                                    ,phoneNum{num},eMail{mail},userName{user},password{pass}{}

QString User::getFirstName() const
{
    return firstName;
}

QString User::getLastName() const
{
    return lastName;
}

void User::setFirstName(const QString &newFirstName)
{
    firstName = newFirstName;
}

void User::setLastName(const QString &newLastName)
{
    lastName = newLastName;
}

int User::getPhoneNum() const
{
    return phoneNum;
}

void User::setPhoneNum(int newPhoneNum)
{
    phoneNum = newPhoneNum;
}

QString User::getEMail() const
{
    return eMail;
}

void User::setEMail(const QString &newEMail)
{
    eMail = newEMail;
}

QString User::getUserName() const
{
    return userName;
}

void User::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

QString User::getPassword() const
{
    return password;
}

void User::setPassword(const QString &newPassword)
{
    password = newPassword;
}

bool User::operator==(const User & lhs)
{
    return this->userName == lhs.getUserName();
}


