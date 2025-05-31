#ifndef HISTORY_H
#define HISTORY_H

#include<QDateTime>
#include<QObject>
#include<vector>
struct History
{
    History(QDateTime d,QString o, QString re,QString r) {date = d;opponent=o;result=re;rounds=r;}

    QDateTime date;
    QString opponent;
    QString result;
    QString rounds;
    bool operator>(const History& lhs){
        return date > lhs.date;
    }
};

std::vector<History> hist;

#endif // HISTORY_H
