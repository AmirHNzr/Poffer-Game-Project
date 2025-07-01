#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include"GameManager.h"
#include <algorithm>
#include <unordered_set>
#include <random>
#include "TimerThread.h"
#include<QTcpSocket>
#include<QTimer>
#include"Users.h"

enum class Hands {
    None        =-1,
    HighCard    = 0,
    OnePair        ,
    TwoPair        ,
    ThreeOfAKind   ,
    Straight       ,
    Flush          ,
    FullHouse      ,
    FourOfAKind    ,
    StraightFlush  ,
    RoyalFlush
};

enum class GamePhase {
    None = -1,
    dealingP1,
    dealingP2,
    SwitchPs

};

struct HandValue {
    Hands            category;
    std::vector<int> tiebreakers;
};

static constexpr const char* RANK_NAMES[13] = {
    "2", "3", "4", "5", "6", "7",
    "8", "9", "10", "Soldier", "Queen", "King", "Bitcoin"
};

static constexpr const char* SUIT_NAMES[4] = {
    "Coin", "Dollar", "Gold", "Diamond"
};

inline void decodeCardIndex(int n, int& suitIndex, int& rankIndex) {
    if (n < 1 || n > 52) {
        suitIndex = -1;
        rankIndex = -1;
        return;
    }
    int zeroBased = n - 1;
    suitIndex = zeroBased / 13;   // 0 = Coin, 1 = Dollar, 2 = Gold, 3 = Diamond
    rankIndex = zeroBased % 13;   // 0 = "2" ,10 = "Queen", 11 = "King", 12 = "Bitcoin"
}


class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(GameManager*, Users* u = nullptr, QObject *parent = nullptr);


    void operator()(const QJsonObject&);
signals:
    void StopTimer();
private:
    GameManager* _gm;
    Users* _db;
    std::vector<QueueEntry*> _sessionPlayers;
    QHash<int,QueueEntry> _playerOrder;
    QHash<QString,std::vector<int>> _playersCards;

    QTcpSocket* _currSock;
    QueueEntry _currPlayer;
    GamePhase _gamePhase = GamePhase::None;


    int _startedSessions;
    int _gameRound;
    int _innerRound;
    bool haltDone;
    std::vector<int> _cards;
    std::vector<int> _drawnCards;
    std::random_device rd;
    TimerThread* _timer;

    void SendData(QJsonObject&,QTcpSocket*);

    void ResetCards();
    void StartGame();
    void ResetSession();
    void AddHistoryToDB(int w, int l, QString reason="");

    void PickFirstPlayer();
    void DrawCards(const int& num);
    void SendCards(QTcpSocket*);
    void DiscardCards();
    bool Halt(QTcpSocket*,int);

    void IncomingCardPICKED(QTcpSocket*);

    HandValue HandEvaluator(const std::vector<int> &cards);
    bool CompareHands(const HandValue& hv1,const HandValue& hv2);
    bool CompareHands(const std::vector<int>& hand1, const std::vector<int>& hand2);
    void RoundRes();
    void MatchRes();

    // Asynchronicity

    QTimer* _timeoutTimer;
    QTimer* _pauseTimer;

    bool _pause;
    QString _pausePlayer;

    QElapsedTimer _elapsed;
    int remainingTimeMs, timeoutCnt;
    void Dealing();
    void SetupTimer(int ms);
    void onPlayerReconnected();
private slots:
    void onTimeout();
    void onPlayerDisconnected();
};

#endif // GAMESESSION_H
