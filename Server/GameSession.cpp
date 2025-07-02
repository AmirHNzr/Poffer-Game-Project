#include "GameSession.h"
#include "qeventloop.h"

const QString handCategoryToString(Hands h) {
    switch (h) {
    case Hands::HighCard:      return "High Card";
    case Hands::OnePair:       return "One Pair";
    case Hands::TwoPair:       return "Two Pair";
    case Hands::ThreeOfAKind:  return "Three of a Kind";
    case Hands::Straight:      return "Straight";
    case Hands::Flush:         return "Flush";
    case Hands::FullHouse:     return "Full House";
    case Hands::FourOfAKind:   return "Four of a Kind";
    case Hands::StraightFlush: return "Straight Flush";
    case Hands::RoyalFlush:    return "Royal Flush";
    default:                   return "Unknown";
    }
}

GameSession::GameSession(GameManager* gm, Users* u,QObject *parent)
    : QObject{parent},_gm{gm},_db{u}
{
    _timeoutTimer = new QTimer(this);
    _timeoutTimer->setSingleShot(true);
    connect(_timeoutTimer, &QTimer::timeout,this, &GameSession::onTimeout);

    _pauseTimer = new QTimer(this);
    _pauseTimer->setSingleShot(true);
    connect(_pauseTimer, &QTimer::timeout,this, &GameSession::onTimeout);
    _pause = false;

    _startedSessions = 0;
    _gameRound = 1;
    _innerRound = 1;

    ResetCards();
    _gamePhase = GamePhase::dealingP1;


}

void GameSession::operator()(const QJsonObject &obj)
{
    auto cmd = obj.value("cmd").toString();
    qDebug() << "[GameSession] received cmd =" << cmd << ", full object =" << obj;

    if(cmd == "GAME_STARTED"){
        qDebug() << "in GAME_STARTED\n";
        _startedSessions++;
        qDebug() << _startedSessions << "\n";

        if(_startedSessions == 2){
            if(!_gm->_sessionPlayers.empty()){
                _sessionPlayers.push_back(_gm->_sessionPlayers.back());
                _gm->_sessionPlayers.pop_back();
                if(!_gm->_sessionPlayers.empty()){
                    _sessionPlayers.push_back(_gm->_sessionPlayers.back());
                    _gm->_sessionPlayers.pop_back();
                }


            }
            for (auto entry : _sessionPlayers) {
                QTcpSocket* sock = entry->socket;
                connect(sock, &QTcpSocket::disconnected,
                        this, &GameSession::onPlayerDisconnected);
                entry->hist.date = QDateTime::currentDateTime();

            }
            _sessionPlayers[0]->hist.opponent = _sessionPlayers[1]->username;
            _sessionPlayers[1]->hist.opponent = _sessionPlayers[0]->username;

            StartGame();

        }
    }
    else if(cmd == "EXIT"){
        if(obj["username"].toString() == _playerOrder[0].username){
            QJsonObject exit;
            exit["cmd"] = "MATCH_RESULT";
            exit["msg"] = "Other player gave up on match and you won";
            SendData(exit,_playerOrder[1].socket);

            AddHistoryToDB(1,0,"by exit");

            ResetSession();
            return;
        }
        else{
            QJsonObject exit;
            exit["cmd"] = "MATCH_RESULT";
            exit["msg"] = "Other player gave up on match and you won";
            SendData(exit,_playerOrder[0].socket);

            AddHistoryToDB(1,0,"by exit");

            ResetSession();
            return;
        }
    }
    else if(cmd == "RECONNECTED"){
        onPlayerReconnected();
    }
    // They have to send username and picked card in order to work correctly
    else if(cmd == "PICKED"){
        qDebug() << ">><< in PICKED";
        int num = obj["card"].toInt();
        auto itr = std::find(_drawnCards.begin(),_drawnCards.end(),num);
        haltDone = true;

        if (itr == _drawnCards.end()) return;
        qDebug() << ">><< in PICKED --- DIDNT RETURN";

        _drawnCards.erase(itr);
        _playersCards[obj["username"].toString()].push_back(num);
        timeoutCnt = 0;
        if(_currPlayer.username == _playerOrder[0].username){
            qDebug() << "now player 2";
            _gamePhase = GamePhase::dealingP2;
            _currPlayer = _playerOrder[1];
            StartGame();
        }
        else{
            qDebug() << "end of round";
            for(auto card:_playersCards[_playerOrder[0].username]){
                qDebug() << card;
            }
            for(auto card:_playersCards[_playerOrder[1].username]){
                qDebug() << card;
            }
            _innerRound++;
            _gamePhase = GamePhase::SwitchPs;
            StartGame();
        }
    }
    else if(cmd == "PAUSED"){
        _pausePlayer = obj["username"].toString();

        QJsonObject pause;
        pause["cmd"] = "PAUSE";
        pause["username"] = _pausePlayer;

        SendData(pause,_playerOrder[0].socket);
        SendData(pause,_playerOrder[1].socket);

        _timeoutTimer->stop();
        _pause = true;
        _pauseTimer->start(20'000);

    }
    else if(cmd == "RESUMED"){
        _timeoutTimer->start();
        _pauseTimer->stop();
        _pause = false;

        QJsonObject pause;
        pause["cmd"] = "RESUME";
        pause["username"] = _pausePlayer;

        SendData(pause,_playerOrder[0].socket);
        SendData(pause,_playerOrder[1].socket);

    }
    else if(cmd == "CHANGE_REQ"){
        if(_innerRound == 1 || _innerRound == 5) return;
        QJsonObject req;
        req["cmd"] = "CHANGE_REQ_RECEIVED";
        if(_playerOrder[0].username == obj["username"].toString())
            SendData(req,_playerOrder[1].socket);
        else
            SendData(req,_playerOrder[0].socket);
    }
    else if(cmd == "CHANGE_REQ_ACCEPTED"){
        ChangeIndexCnt = 0;
        QJsonObject req;
        req["cmd"] = "CHANGE_ACCEPTED";
        SendData(req,_playerOrder[0].socket);
        SendData(req,_playerOrder[1].socket);
    }
    else if(cmd == "CHANGE_RECEIVED"){
        if(obj["username"].toString() == _playerOrder[0].username){
            ChangeIndex[0] = obj["index"].toInt();
            ChangeIndexCnt++;
        }
        else{
            ChangeIndex[1] = obj["index"].toInt();
            ChangeIndexCnt++;
        }
        if(ChangeIndexCnt == 2){
            int temp = _playersCards[_playerOrder[0].username].at(ChangeIndex[0]);
            _playersCards[_playerOrder[0].username].at(ChangeIndex[0]) = _playersCards[_playerOrder[1].username].at(ChangeIndex[1]);
            _playersCards[_playerOrder[1].username].at(ChangeIndex[1]) = temp;

            QJsonObject obj2;
            obj2["cmd"] = "CHANGE_DONE";
            obj2["card"] = _playersCards[_playerOrder[0].username].at(ChangeIndex[0]);
            SendData(obj2,_playerOrder[0].socket);
            obj2["card"] = _playersCards[_playerOrder[1].username].at(ChangeIndex[1]);
            SendData(obj2,_playerOrder[1].socket);

            ChangeIndexCnt=0;
        }
    }
}

void GameSession::onPlayerDisconnected()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    QString user;
    if (sock == _playerOrder[0].socket) user = _playerOrder[0].username;
    else if (sock == _playerOrder[1].socket) user = _playerOrder[1].username;
    else return;

    _pausePlayer = user;
    _pause = true;

    QJsonObject pauseMsg;
    pauseMsg["cmd"]      = "DISCONNECTION";
    pauseMsg["username"] = user;
    SendData(pauseMsg, _playerOrder[0].socket);
    SendData(pauseMsg, _playerOrder[1].socket);

    _pauseTimer->start(20'000);
}

void GameSession::onPlayerReconnected()
{
    // QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    // QString user;
    // if (sock == _playerOrder[0].socket) user = _playerOrder[0].username;
    // else if (sock == _playerOrder[1].socket) user = _playerOrder[1].username;
    // else return;

    // if (!_pause || user != _pausePlayer) return;
    _pauseTimer->stop();
    _pause = false;

    QJsonObject resumeMsg;
    resumeMsg["cmd"]      = "RECONNECTION";
    // resumeMsg["username"] = user;
    SendData(resumeMsg, _playerOrder[0].socket);
    SendData(resumeMsg, _playerOrder[1].socket);
}

void GameSession::SendData(QJsonObject &obj,QTcpSocket* _sock)
{
    QJsonDocument doc(obj);
    QByteArray   bytes = doc.toJson(QJsonDocument::Compact) + "\n";
    if (_sock && _sock->state() == QAbstractSocket::ConnectedState) {
        qDebug() << ">>>>> cmd:" + obj["cmd"].toString();
        qDebug() << ">>>>> Bytes:" + bytes;
        qDebug() << ">>>>> Socket:" << _sock->socketDescriptor();
        _sock->write(bytes);
        _sock->flush();
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


    if(_gameRound <= 3){
        if(_playerOrder[1].wins == 2 || _playerOrder[0].wins == 2){
            MatchRes();
            return;}
        if(_innerRound <=5){
            qDebug() << "innerRound no.:" << _innerRound;
            qDebug() << "Round no.:" << _gameRound;
            if(_innerRound == 1 && _gamePhase == GamePhase::dealingP1){
                qDebug() << "<<<>>>Preparing inRound:" << _gameRound;
                timeoutCnt = 0;
                PickFirstPlayer();
                ResetCards();
                _playersCards[_playerOrder[0].username].clear();
                _playersCards[_playerOrder[1].username].clear();
            }
            switch(_gamePhase){
            case GamePhase::dealingP1:
                qDebug() << "???????dealing P1:";
                Dealing();
                break;
            case GamePhase::dealingP2:
                qDebug() << "???????dealing P2";
                Dealing();
                break;
            case GamePhase::SwitchPs:
                qDebug() << "???????Switch up";
                Dealing();
            case GamePhase::None:
                break;
            }
        }
        else{
            qDebug() << "{}>>>>on displaying inround res";
            qDebug() << _innerRound;
            RoundRes();
            ResetCards();
            _innerRound = 1;
            _gameRound++;
            StartGame();
        }

    }else{
        MatchRes();
    }
}

void GameSession::Dealing(){
    if(_gamePhase == GamePhase::dealingP1){
        _currPlayer = _playerOrder[0];
        DrawCards(7);
        DiscardCards();
        SendCards(_playerOrder[0].socket);
        SetupTimer(20'000);
    }
    if(_gamePhase == GamePhase::dealingP2){
        //DiscardCards();
        SendCards(_playerOrder[1].socket);
        SetupTimer(20'000);
    }
    if(_gamePhase == GamePhase::SwitchPs){
        _drawnCards.clear();
        std::swap(_playerOrder[0],_playerOrder[1]);
        _gamePhase = GamePhase::dealingP1;
        StartGame();
    }
}

void GameSession::SetupTimer(int ms){
    _timeoutTimer->start(ms);

}

void GameSession::onTimeout()
{
    if(_pause == true){
        QJsonObject obj;

        if(_playerOrder[0].username != _pausePlayer)
            std::swap(_playerOrder[0],_playerOrder[1]);

        obj["cmd"] = "MATCH_RESULT";
        obj["msg"] = "Match ended and you won";
        SendData(obj,_playerOrder[1].socket);
        obj["msg"] = "Match ended and you lost";
        SendData(obj,_playerOrder[0].socket);

        AddHistoryToDB(1,0,"by timeout");

        _pause = false;
        ResetSession();
        return;
    }
    if(timeoutCnt == 0){
        QJsonObject almost;
        almost["cmd"] = "ALMOST_TIMEOUT";
        SendData(almost, _currPlayer.socket);
        SetupTimer(10'000);
        timeoutCnt++;
    }
    else if(timeoutCnt == 1){
        QJsonObject almost;
        almost["cmd"] = "TIMEOUT";
        SendData(almost, _currPlayer.socket);

        // almost["cmd"] = "PICKED";
        // almost["username"] = _currPlayer.username;
        // almost["card"] = _drawnCards.back();
        // (*this)(almost);
        // haltDone = false;

        timeoutCnt = 0;
    }

}


void GameSession::RoundRes(){
    QJsonObject obj;
    // QJsonArray opponent,opponent2;
    obj["cmd"] = "ROUND_RESULT";
    HandValue P0 = HandEvaluator(_playersCards[_playerOrder[0].username]);
    HandValue P1 = HandEvaluator(_playersCards[_playerOrder[1].username]);
    bool result = CompareHands(P0,P1);

    qDebug() << "/|\\ Player1 hand:" << handCategoryToString(P0.category);
    qDebug() << "/|\\ Player2 hand:" << handCategoryToString(P1.category);
    obj[_playerOrder[0].username] = handCategoryToString(P0.category);
    obj[_playerOrder[1].username] = handCategoryToString(P1.category);
    qDebug() << "/|\\ Player1 obj hand:" << obj[_playerOrder[0].username].toString();
    qDebug() << "/|\\ Player2 obj hand:" << obj[_playerOrder[1].username].toString();

    // for(auto& card:_playersCards[_playerOrder[0].username])
    //     opponent.append(card);
    obj["opponent"] = handCategoryToString(P1.category);
    qDebug() << "/|\\ opponent obj hand:" << obj["opponent"].toString();

    obj["result"] = result ? "won":"lost";
    SendData(obj,_playerOrder[0].socket);

    //result ? _playerOrder[0].wins++ : _playerOrder[1].wins++;

    if(result){
        _playerOrder[0].hist.rounds[_gameRound-1] = "won";
        _playerOrder[1].hist.rounds[_gameRound-1] = "lost";

        _playerOrder[0].wins++;
    }
    else{
        _playerOrder[1].hist.rounds[_gameRound-1] = "won";
        _playerOrder[0].hist.rounds[_gameRound-1] = "lost";

        _playerOrder[1].wins++;
    }

    // for(auto& card:_playersCards[_playerOrder[1].username])
    //     opponent2.append(card);
    obj["opponent"] = handCategoryToString(P0.category);
    qDebug() << "/|\\ opponent obj hand:" << obj["opponent"].toString();

    obj["result"] = result ? "lost":"won";
    SendData(obj,_playerOrder[1].socket);

    if(_playerOrder[0].wins == 2 || _playerOrder[1].wins == 2){
        Halt(nullptr,2000);
        return;}
    Halt(nullptr,2000);
    qDebug() << "{}>>>>end of displaying inround res";
    qDebug() << _innerRound;
}

void GameSession::MatchRes(){
    qDebug() << "[MATCH RES]";
    if(_playerOrder[1].wins == 2){
        QJsonObject obj;
        obj["cmd"] = "MATCH_RESULT";
        obj["msg"] = "Match ended and you won";
        SendData(obj,_playerOrder[1].socket);
        obj["msg"] = "Match ended and you lost";
        SendData(obj,_playerOrder[0].socket);


        AddHistoryToDB(1,0);
    }
    else if(_playerOrder[0].wins == 2){
        QJsonObject obj;
        obj["cmd"] = "MATCH_RESULT";
        obj["msg"] = "Match ended and you won";
        SendData(obj,_playerOrder[0].socket);
        obj["msg"] = "Match ended and you lost";
        SendData(obj,_playerOrder[1].socket);


        AddHistoryToDB(0,1);
    }


    ResetSession();
}

void GameSession::ResetSession()
{
    _gm->setQueueZero();
    _startedSessions = 0;
    _gameRound = 1;
    _innerRound = 1;
    haltDone = false;
    ResetCards();
    _drawnCards.clear();
    _sessionPlayers.clear();
    _playerOrder.clear();
    _playersCards.clear();

    _timeoutTimer->stop();
    _pauseTimer->stop();
}

void GameSession::AddHistoryToDB(int w, int l,QString reason)
{
    _playerOrder[w].hist.result = "won "+reason;
    _playerOrder[l].hist.result = "lost "+reason;
    _db->AddHistory(_playerOrder[0].username,_playerOrder[0].hist);
    _db->AddHistory(_playerOrder[1].username,_playerOrder[1].hist);
}

void GameSession::PickFirstPlayer()
{
    if(_gameRound == 1){
        std::mt19937 gen(rd());

        std::shuffle(_cards.begin() +39 , _cards.end() , gen);
        int firstDiamond  = _cards[39];
        int secondDiamond = _cards[40];

        QJsonObject obj;
        obj["cmd"] = "PLAYERS_ORDER";

        if(firstDiamond > secondDiamond){
            _playerOrder.insert(0,*_sessionPlayers.at(0));
            obj["yours"] = firstDiamond;
            obj["opponents"] = secondDiamond;
            obj["result"] = "first";
            SendData(obj,_sessionPlayers[0]->socket);

            _playerOrder.insert(1,*_sessionPlayers.at(1));
            obj["yours"] = secondDiamond;
            obj["opponents"] = firstDiamond;
            obj["result"] = "second";
            SendData(obj,_sessionPlayers[1]->socket);

            Halt(nullptr,2000);

        }
        else{
            _playerOrder.insert(0,*_sessionPlayers.at(1));
            obj["yours"] = secondDiamond;
            obj["opponents"] = firstDiamond;
            obj["result"] = "first";
            SendData(obj,_sessionPlayers[1]->socket);

            _playerOrder.insert(1,*_sessionPlayers.at(0));
            obj["yours"] = firstDiamond;
            obj["opponents"] = secondDiamond;
            obj["result"] = "second";
            SendData(obj,_sessionPlayers[0]->socket);

            Halt(nullptr,2000);
        }
    }
    else{
        QJsonObject obj;
        obj["cmd"] = "PLAYERS_ORDER";
        obj["result"] = "first";
        SendData(obj,_playerOrder[0].socket);

        obj["result"] = "second";
        SendData(obj,_playerOrder[1].socket);

        Halt(nullptr,2000);


    }


}

void GameSession::DrawCards(const int& num)
{
    std::mt19937 gen(rd());
    std::shuffle(_cards.begin(),_cards.end(),gen);

    for(int i=0;i<num;i++)
        _drawnCards.push_back(_cards[i]);

}

void GameSession::SendCards(QTcpSocket* _sock)
{
    int i =0;
    QJsonObject obj;
    obj["cmd"] = "CARD_BATCH";
    for(auto& card:_drawnCards){
        obj[QString::number(i)] = card;
        i++;
    }
    _currSock = _sock;
    SendData(obj,_sock);
}

void GameSession::DiscardCards()
{
    std::unordered_set<int> removeSet(_drawnCards.begin(), _drawnCards.end());

    auto newEnd = std::remove_if(
        _cards.begin(),
        _cards.end(),
        [&](int x) { return removeSet.count(x) > 0; });

    _cards.erase(newEnd, _cards.end());
}

bool GameSession::Halt(QTcpSocket* _sock,int halt)
{
    _timer = new TimerThread(halt, this);
    QEventLoop    loop;
    bool playerHasData = false;

    QObject::connect(_timer,  &TimerThread::timeout, &loop, &QEventLoop::quit);
    QObject::connect(this, &GameSession::StopTimer, &loop, &QEventLoop::quit);
    QObject::connect(this, &GameSession::StopTimer, _timer,&TimerThread::timeout); //added this  --> may lead to new problems
    QObject::connect(this, &GameSession::StopTimer, &loop,[&](){playerHasData = true;loop.quit();});

    _timer->start();
    loop.exec();



    if (_timer->isRunning()) {
        _timer->quit();
        _timer->wait();
    }
    delete _timer;

    return playerHasData;
}

/*void GameSession::IncomingCardPICKED(QTcpSocket *sock)
{
    // if (sock->waitForReadyRead(20000)) {
    //     QByteArray line = sock->readLine().trimmed();
    //     QJsonParseError parseError;
    //     QJsonDocument  doc    = QJsonDocument::fromJson(line, &parseError);
    //     if (!parseError.error && doc.isObject()) {
    //         QJsonObject obj = doc.object();
    //         if (obj.value("cmd").toString() == "PICKED")
    //             (*this)(obj);

    //     }
    //     haltDone = true;
    //     return;

    // }
    // else{
    //     QJsonObject almost;
    //     almost["cmd"] = "ALMOST_TIMEOUT";
    //     SendData(almost, sock);
    // }

    // if (sock->waitForReadyRead(10000)) {
    //     QByteArray line2 = sock->readLine().trimmed();
    //     QJsonParseError parseError2;
    //     QJsonDocument  doc2= QJsonDocument::fromJson(line2, &parseError2);
    //     if (!parseError2.error && doc2.isObject()) {
    //         QJsonObject obj2 = doc2.object();
    //         if (obj2.value("cmd").toString() == "PICKED")
    //             (*this)(obj2);
    //         haltDone = true;
    //         return;

    //     }
    // }
    // else{
    //     QJsonObject almost;
    //     almost["cmd"] = "ALMOST_TIMEOUT";
    //     SendData(almost, sock);

    //     haltDone = false;
    // }
}*/

HandValue GameSession::HandEvaluator(const std::vector<int>& cards)
{
    HandValue hv;

    std::vector<int> handOne = cards;

    if(handOne.size() != 5){
        qWarning() << "something is not right";
        hv.category = Hands::HighCard;
        hv.tiebreakers = { -1 };
        return hv;    }

    int suitsCount[4]{0};
    int ranksCount[13]{0};

    int suits[5]{0};
    int ranks[5]{0};

    bool isFlush = false;
    bool isStraight = false;


    for(int i=0;i<5;i++){
        decodeCardIndex(handOne[i],suits[i],ranks[i]);

        suitsCount[suits[i]]++;
        ranksCount[ranks[i]]++;
    }

    // Flush check
    for(int s=0;s<4;s++){
        if(suitsCount[s] == 5){
            isFlush = true;
        }
    }

    // to check 4-of-a-kind, fullhouse, two-pair, pair
    // we save each rank and corresponding freq
    std::vector<std::pair<int,int>> freqCounts;
    for (int r = 0; r < 13; ++r) {
        if (ranksCount[r] > 0) {
            freqCounts.push_back({ranksCount[r],r});
        }
    }
    std::sort(freqCounts.begin(), freqCounts.end(),[](auto &a, auto &b){
        if (a.first != b.first) return (a.first > b.first);
        else                    return (a.second > b.second);});

    // Check for Straight
    // criteria: 1- Five distinct cards, 2- Five distinct consecutive cards
    // one pitfall: Ace,2,3,4,5
    std::vector<int> distinctRanks;
    distinctRanks.reserve(5);
    for (int r = 0; r < 13; ++r) {
        if (ranksCount[r] == 1) {
            distinctRanks.push_back(r);
        }
    }
    std::sort(distinctRanks.begin(), distinctRanks.end());
    int highestStraightRank = -1;

    if(distinctRanks.size() == 5){
        if (distinctRanks.back() - distinctRanks.front() == 4){
            isStraight = true;
            highestStraightRank = distinctRanks.back();}
        else if(distinctRanks[0] == 0 &&
                 distinctRanks[1] == 1 &&
                 distinctRanks[2] == 2 &&
                 distinctRanks[3] == 3 &&
                 distinctRanks[4] == 12){
            isStraight = true;
            highestStraightRank = 3;}
    }

    //    in descending order
    //    – Royal Flush:     straight + flush + top five are 10,J,Q,K,A  (i.e. ranks {8,9,10,11,12})
    //    – Straight Flush:  straight + flush
    //    – Four of a Kind:  freqCounts == {4,1}
    //    – Full House:      freqCounts == {3,2}
    //    – Flush:           isFlush
    //    – Straight:        isStraight
    //    – Three of a Kind: freqCounts == {3,1,1}
    //    – Two Pair:        freqCounts == {2,2,1}
    //    – One Pair:        freqCounts == {2,1,1,1}
    //    – High Card:       none of the above
    qDebug() << "@@@@ Hand features:\n";
    qDebug() << isStraight <<"\n";
    qDebug() << isFlush <<"\n";
    qDebug() << highestStraightRank <<"\n";
    for(auto freqs: freqCounts)
        qDebug() << freqs <<"-\n";


    if (isStraight && isFlush) {
        // check for 10, J, Q, K, A:
        std::vector<int> royalCheck = {8, 9, 10, 11, 12};
        std::sort(distinctRanks.begin(), distinctRanks.end());
        if (distinctRanks == royalCheck) {
            hv.category = Hands::RoyalFlush;
            hv.tiebreakers.clear();
            return hv;
        }
        // Else → Straight Flush
        hv.category = Hands::StraightFlush;
        hv.tiebreakers = { highestStraightRank };
        return hv;
    }

    else if (freqCounts[0].first == 4) {
        hv.category = Hands::FourOfAKind;
        int rankOfQuad = freqCounts[0].second;
        int kicker     = freqCounts[1].second;  // the one with count == 1
        hv.tiebreakers = { rankOfQuad, kicker };
        return hv;
    }
    else if (freqCounts[0].first == 3 && freqCounts[1].first == 2) {
        hv.category = Hands::FullHouse;
        int rankOfTrip = freqCounts[0].second;
        int rankOfPair = freqCounts[1].second;
        hv.tiebreakers = { rankOfTrip, rankOfPair };
        return hv;
    }
    else if (isFlush) {
        hv.category = Hands::Flush;
        // tiebreakers = the five ranks sorted descending
        std::vector<int> allRanks(5);
        for (int i = 0; i < 5; ++i) allRanks[i] = ranks[i];
        std::sort(allRanks.begin(), allRanks.end(), std::greater<int>());
        hv.tiebreakers = allRanks;
        return hv;
    }
    else if (isStraight) {
        hv.category = Hands::Straight;
        hv.tiebreakers = { highestStraightRank };
        return hv;
    }
    else if (freqCounts[0].first == 3) {
        hv.category = Hands::ThreeOfAKind;
        int rankOfTrip = freqCounts[0].second;
        // Collect kickers:
        std::vector<int> kickers;
        for (int i = 1; i < 3; ++i) {  // countRankList[1..2] each have first==1
            kickers.push_back(freqCounts[i].second);
        }
        std::sort(kickers.begin(), kickers.end(), std::greater<int>());
        hv.tiebreakers = { rankOfTrip, kickers[0], kickers[1] };
        return hv;
    }
    else if (freqCounts[0].first == 2 && freqCounts[1].first == 2) {
        hv.category = Hands::TwoPair;
        int highPair = std::max(freqCounts[0].second, freqCounts[1].second);
        int lowPair  = std::min(freqCounts[0].second, freqCounts[1].second);
        int kicker   = freqCounts[2].second;  // the one with count==1
        hv.tiebreakers = { highPair, lowPair, kicker };
        return hv;
    }
    else if (freqCounts[0].first == 2) {
        hv.category = Hands::OnePair;
        int pairRank = freqCounts[0].second;
        std::vector<int> kickers;
        for (int i = 1; i < 4; ++i) {  // countRankList[1..3] each have first==1
            kickers.push_back(freqCounts[i].second);
        }
        std::sort(kickers.begin(), kickers.end(), std::greater<int>());
        hv.tiebreakers = { pairRank,
                          kickers[0],
                          kickers[1],
                          kickers[2] };
        return hv;
    }
    else {
        hv.category = Hands::HighCard;
        std::vector<int> sortedRanks(5);
        for (int i = 0; i < 5; ++i) sortedRanks[i] = ranks[i];
        std::sort(sortedRanks.begin(), sortedRanks.end(), std::greater<int>());
        hv.tiebreakers = sortedRanks;
        return hv;
    }
}

bool GameSession::CompareHands(const HandValue &h1, const HandValue &h2)
{
    if (h1.category != h2.category) {
        return (h1.category > h2.category) ? true : false;
    }
    // Same category → compare tiebreakers lexicographically (each vector sorted desc)
    const auto &v1 = h1.tiebreakers;
    const auto &v2 = h2.tiebreakers;
    for (size_t i = 0; i < std::min(v1.size(), v2.size()); ++i) {
        if (v1[i] != v2[i]) {
            return (v1[i] > v2[i]) ? true : false;
        }
    }

    return 0;
}

bool GameSession::CompareHands(const std::vector<int> &hand1, const std::vector<int> &hand2)
{
    HandValue hv1 = HandEvaluator(hand1);
    HandValue hv2 = HandEvaluator(hand2);
    return CompareHands(hv1, hv2);
}




