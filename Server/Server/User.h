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
    bool operator>(const History& lhs){
        return date > lhs.date;
    }
    History& operator=(const History& other)
    {
        if (this != &other) {
            date     = other.date;
            opponent = other.opponent;
            result   = other.result;
            for (int i = 0; i < 3; ++i)
                rounds[i] = other.rounds[i];
        }
        return *this;
    }
    History(const History& other)
        : date(other.date)
        , opponent(other.opponent)
        , result(other.result)
    {
        for (int i = 0; i < 3; ++i)
            rounds[i] = other.rounds[i];
    }
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
    std::vector<History> getHistory() const;
    void setHistory(const std::vector<History> &newHistory);

private:
    QString firstName,lastName;
    int phoneNum;
    QString eMail;
    QString userName,password;
    std::vector<History> history;

};

#endif // USER_H
