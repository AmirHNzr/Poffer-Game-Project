#include "GameSession.h"

GameSession::GameSession(GameManager* gm,QObject *parent)
    : QObject{parent},_gm{gm}
{
    _sessionPlayers = _gm->sessionPlayers();
    _startedSessions = 0;
    _gameRound = 1;

    ResetCards();


}

void GameSession::operator()(const QJsonObject &obj)
{
    auto cmd = obj.value("cmd").toString();

    if(cmd == "GAME_STARTED"){
        _sessionPlayers = _gm->sessionPlayers();
        _startedSessions++;
        if(_startedSessions == 2)
            StartGame();
    }
}

void GameSession::SendData(QJsonObject &obj,QTcpSocket* _sock)
{
    QJsonDocument doc(obj);
    QByteArray   bytes = doc.toJson(QJsonDocument::Compact);
    auto cmd = obj.value("cmd").toString();

    if(cmd == "PLAYERS_ORDER"){
        if (_sock && _sock->state() == QAbstractSocket::ConnectedState) {
            _sock->write(bytes);
            _sock->flush();
        }
    }

}

void GameSession::ResetCards()
{
    _cards.clear();
    _cards.reserve(52);
    for(int i=1;i<=52;i++)
        _cards.push_back(i);
}

void GameSession::StartGame()
{
    while(_gameRound <= 1){
        /*
         * 1-Send random cards to choose first player
         * 2-Add first player and sec to QHash
         * 3-Go in the inner loop of game (loop for 5 time)
         * in each inner loop:
         * 4-Send first player batch of 7 cards
         * 5-Receive the chosen card
         * 6-Propagate the 6 remaining to another player
         * 7-Receive the chosen card and discard the 5 remaining
         * 8-For other inner loops shuffle the remaining cards and switch the first and second player
         * 9-after 5 inner loops we have 5 card for each player
         * 10-specify the hand they have and send a signal to winner and loser
         * 11-Add win and lose for corresponding user (make a profile for them)
         */

        PickFirstPlayer();
        _gameRound++;



    }


}

void GameSession::PickFirstPlayer()
{
    std::mt19937 gen(rd());

    std::shuffle(_cards.begin() +39 , _cards.end() , gen);
    int firstDiamond  = _cards[39];
    int secondDiamond = _cards[40];

    QJsonObject obj;
    obj["cmd"] = "PLAYERS_ORDER";

    if(firstDiamond > secondDiamond){
        _playerOrder.insert(0,_sessionPlayers.at(0));
        obj["yours"] = firstDiamond;
        obj["opponents"] = secondDiamond;
        obj["result"] = "first";
        SendData(obj,_sessionPlayers[0].socket);

        _playerOrder.insert(1,_sessionPlayers.at(1));
        obj["yours"] = secondDiamond;
        obj["opponents"] = firstDiamond;
        obj["result"] = "second";
        SendData(obj,_sessionPlayers[1].socket);

    }
    else{
        _playerOrder.insert(0,_sessionPlayers.at(1));
        obj["yours"] = secondDiamond;
        obj["opponents"] = firstDiamond;
        obj["result"] = "first";
        SendData(obj,_sessionPlayers[1].socket);

        _playerOrder.insert(1,_sessionPlayers.at(0));
        obj["yours"] = firstDiamond;
        obj["opponents"] = secondDiamond;
        obj["result"] = "second";
        SendData(obj,_sessionPlayers[0].socket);
    }


}
