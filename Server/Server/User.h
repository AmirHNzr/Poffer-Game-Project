#ifndef USER_H
#define USER_H
#include<QString>
#include<QJsonObject>
#include<queue>
#include <QDateTime>


struct History
{
    History() {date = QDateTime::currentDateTime();}

    QDateTime date;
    QString opponent;
    QString result;
    QString rounds[3];

};

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
    std::queue<History> getHistory() const;
    void setHistory(const std::queue<History> &newHistory);

private:
    QString firstName,lastName;
    int phoneNum;
    QString eMail;
    QString userName,password;
    std::queue<History> history;

};

#endif // USER_H
