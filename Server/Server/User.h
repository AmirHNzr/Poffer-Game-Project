#ifndef USER_H
#define USER_H

#include<unordered_map>
#include<algorithm>
#include<QString>

class User
{
public:
    User();
    User(QString fName,QString lName,int num,QString mail,QString user,QString pass);

    QString getFirstName() const;
    QString getLastName() const;

    void setFirstName(const QString &newFirstName);
    void setLastName(const QString &newLastName);

    int getPhoneNum() const;
    void setPhoneNum(int newPhoneNum);

    QString getEMail() const;
    void setEMail(const QString &newEMail);

    QString getUserName() const;
    void setUserName(const QString &newUserName);

    QString getPassword() const;
    void setPassword(const QString &newPassword);

    bool operator==(const User&);
private:
    QString firstName,lastName;
    int phoneNum;
    QString eMail;
    QString userName,password;

};

#endif // USER_H
