#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include"GameManager.h"
#include <algorithm>
#include <random>
class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(GameManager*,QObject *parent = nullptr);


    void operator()(const QJsonObject&);
signals:

private:
    GameManager* _gm;
    std::vector<QueueEntry> _sessionPlayers;
    QHash<int,QueueEntry> _playerOrder;

    int _startedSessions;
    int _gameRound;
    std::vector<int> _cards;
    std::random_device rd;

    void SendData(QJsonObject&,QTcpSocket*);

    void ResetCards();
    void StartGame();

    void PickFirstPlayer();
};

#endif // GAMESESSION_H
