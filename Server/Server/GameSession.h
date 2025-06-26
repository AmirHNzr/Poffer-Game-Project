#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include"GameManager.h"
#include <algorithm>
#include <unordered_set>
#include <random>
#include "TimerThread.h"




class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(GameManager*,QObject *parent = nullptr);


    void operator()(const QJsonObject&);
signals:
    void StopTimer();
private:
    GameManager* _gm;
    std::vector<QueueEntry> _sessionPlayers;
    QHash<int,QueueEntry> _playerOrder;
    QHash<QString,std::vector<int>> _playersCards;


    int _startedSessions;
    int _gameRound;
    int _innerRound;
    std::vector<int> _cards;
    std::vector<int> _drawnCards;
    std::random_device rd;
    TimerThread* _timer;

    void SendData(QJsonObject&,QTcpSocket*);

    void ResetCards();
    void StartGame();

    void PickFirstPlayer();
    void DrawCards(const int& num);
    void SendCards(QTcpSocket*);
    void DiscardCards();
    bool Halt(QTcpSocket*,int);
};

#endif // GAMESESSION_H
