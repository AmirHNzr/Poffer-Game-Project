#ifndef USER_H
#define USER_H
#include<QString>
#include<QJsonObject>
#include<queue>
#include <QDateTime>


struct History
{
    History() {date = QDateTime::currentDateTime();}

    History(QString date,QString opp,QString res,QString r1,QString r2,QString r3){
        this->date = QDateTime::fromString(date, "yyyy-MM-dd HH:mm:ss");
        opponent = opp;
        result = res;
        rounds[0] = r1;
        rounds[1] = r2;
        rounds[2] = r3;

    }

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
    User(QString fName,QString lName,QString num,QString mail,QString user,QString pass);

    QString getFirstName() const;
    QString getLastName() const;

    void setFirstName(const QString &newFirstName);
    void setLastName(const QString &newLastName);

    QString getPhoneNum() const;
    void setPhoneNum(QString newPhoneNum);

    QString getEMail() const;
    void setEMail(const QString &newEMail);

    QString getUserName() const;
    void setUserName(const QString &newUserName);

    QString getPassword() const;
    void setPassword(const QString &newPassword);

    bool operator==(const User&);
    std::vector<History> getHistory() const;
    void setHistory(const std::vector<History> &newHistory);

    void AddHistory(const History);

private:
    QString firstName,lastName;
    QString phoneNum;
    QString eMail;
    QString userName,password;
    std::vector<History> history;

};

#endif // USER_H
