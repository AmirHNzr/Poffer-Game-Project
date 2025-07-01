#ifndef HISTORY_H
#define HISTORY_H

#include<QDateTime>
#include<QObject>
#include<vector>
struct History
{
    History() {date = QDateTime::currentDateTime();}
    History(QDateTime time,QString opp,QString res, QString* rnd){
        date = time;
        opponent = opp;
        result = res;
        for (int i = 0; i < 3; ++i) {
            rounds[i] = rnd[i];
        }
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

extern std::vector<History> hist;

#endif // HISTORY_H
