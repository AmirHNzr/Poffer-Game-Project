#include "GamePage.h"
#include "ui_GamePage.h"

static constexpr const char* RANK_NAMES[13] = {
    "2", "3", "4", "5", "6", "7",
    "8", "9", "10", "Soldier", "Queen", "King", "Bitcoin"
};

static constexpr const char* SUIT_NAMES[4] = {
    "Coin", "Dollar", "Gold", "Diamond"
};

inline void decodeCardIndex(int n, int& suitIndex, int& rankIndex) {
    //sanity check
    if (n < 1 || n > 52) {
        suitIndex = -1;
        rankIndex = -1;
        return;
    }
    int zeroBased = n - 1;
    suitIndex = zeroBased / 13;   // 0=coin,1=dollar,2=gold,3=diamonds
    rankIndex = zeroBased % 13;
}

GamePage::GamePage(UserController* c,PlayerInfo* p,QJsonArray ps,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GamePage)
    , _player(p)
    , _players(ps)
    , _controller(c)
    , _scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(_scene);
    SetupFadingMsg();

    if(_players.first() == _player->username())
        opponent = _players.last().toString();
    opponent = _players.first().toString();

    m_cards.clear();
    m_visibleCards.clear();

    QSize targetSize(100, 100);
    //Preload all 52 face‐side pixmaps into m_facePixmaps
    for (int i = 0; i < 52; ++i) {
        int n = i + 1;
        int suitIdx, rankIdx;
        decodeCardIndex(n, suitIdx, rankIdx);

        // Build a resource path like ":/cards/Coin-2.JPG"
        QString path = QString(":/Images/cards/%1-%2.JPG")
                           .arg(SUIT_NAMES[suitIdx])
                            .arg(RANK_NAMES[rankIdx]);

        QPixmap px;
        if (!px.load(path)) {
            qWarning() << "Could not load card image at" << path;
        }
        px = px.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << "card:"<< n<<"<><><><>px cahce:"<<px.cacheKey();
        m_facePixmaps[i] = px;
    }

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    connect(_controller,&UserController::jsonReceived,this,&GamePage::SessionOrders);


}

GamePage::~GamePage()
{
    delete ui;
    for (auto item : m_visibleCards) {
        delete item;
    }
    m_visibleCards.clear();
}

void GamePage::ShowCards(int xOffset=100,int yOffset=150)
{
    int x=-200;
    int y=-200;
    for(auto& card:m_visibleCards){
        if (card->scene() == _scene) {
            _scene->removeItem(card);
        }
        qDebug() << "<> Showing:" << card->getNum() << "<> pixmap:" << card->pixmap().cacheKey();
        card->setPos(x,y);
        _scene->addItem(card);
        x+=xOffset;
        y-=yOffset;
    }
}

void GamePage::ShowMainCards(int xOffset)
{
    if(m_cards.empty()) return;
    int x=-100;
    int y=100;
    qDebug() << "show main crash";
    for(auto& card:m_cards){
        if (m_cardTimelines.contains(card)) {
            QTimeLine* oldTl = m_cardTimelines.value(card);
            if (oldTl->state() == QTimeLine::Running)
                oldTl->stop();
            oldTl->deleteLater();
            m_cardTimelines.remove(card);
        }

        if (card->scene() == _scene) {
            // If it’s already in the scene at some old position, remove it:
            _scene->removeItem(card);
        }
        card->setPos(x,y);
        _scene->addItem(card);
        x+=xOffset;
    }
}

void GamePage::SetupCardConnections(CardItem* card)
{
    connect(card, &CardItem::doubleClicked, this,  &GamePage::CardSelected);
    connect(card, &CardItem::doubleClicked, this,  [this,card](){
        QJsonObject obj;
        obj["cmd"] = "PICKED";
        obj["username"] = _player->username();
        obj["card"] = card->getNum();
        qDebug() << "++++++Num in connection:" << card->getNum();
        _controller->sendJson(obj);
    });

}

void GamePage::stopAllAnimations()
{
    for (auto it = m_cardTimelines.begin(); it != m_cardTimelines.end(); ++it) {
        QTimeLine *tl = it.value();
        if (tl->state() == QTimeLine::Running) {
            tl->stop();
        }
        tl->deleteLater();
    }
    m_cardTimelines.clear();
}

void GamePage::SetupFadingMsg()
{
    m_overlayLabel = new QLabel(this);
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_fadeAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", this);

    m_overlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_overlayLabel->setAlignment(Qt::AlignCenter);
    m_overlayLabel->setStyleSheet(R"(
        background-color: rgba(0, 0, 0, 128); /* semi-transparent black */
        color: white;
        font: bold 36px;
    )");
    m_overlayLabel->setGraphicsEffect(m_opacityEffect);

    // Make sure the label covers the entire client area:
    m_overlayLabel->setGeometry(rect());
    m_overlayLabel->hide();

}

void GamePage::showFadingMessage(const QString &text, int fadeInMs, int stayMs, int fadeOutMs)
{
    //Set the text:
    m_overlayLabel->setText(text);
    m_overlayLabel->raise();
    m_overlayLabel->show();

    //fade in 0 -> 1
    m_fadeAnimation->stop();
    m_fadeAnimation->setDuration(fadeInMs);
    m_fadeAnimation->setStartValue(0.0);
    m_fadeAnimation->setEndValue(1.0);

    //After fade in, wait for stayMs, then fade out:
    connect(m_fadeAnimation, &QPropertyAnimation::finished, this, [=]() {
        //halt for stayMs, then fade out:
        QTimer::singleShot(stayMs, this, [=]() {
            //fade 1 -> 0:
            m_fadeAnimation->disconnect(); // disconnect old finished() slot
            m_fadeAnimation->setDuration(fadeOutMs);
            m_fadeAnimation->setStartValue(1.0);
            m_fadeAnimation->setEndValue(0.0);

            connect(m_fadeAnimation, &QPropertyAnimation::finished, this, [=]() {
                //after fade out, hide the label
                m_overlayLabel->hide();
                m_fadeAnimation->disconnect();
            });
            m_fadeAnimation->start();
        });
    });

    m_fadeAnimation->start();
}

void GamePage::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    //Resize sceneRect(0,0)→(viewportWidth, viewportHeight)
    const QSize vp = ui->graphicsView->viewport()->size();
    _scene->setSceneRect(0, 0, vp.width(), vp.height());
}

void GamePage::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    //we have to send an "GAME_STARTED" json;
    QJsonObject obj;
    obj["cmd"] = "GAME_STARTED";
    _controller->sendJson(obj);

}

void GamePage::onCardClicked(CardItem *card)
{
    QPointF start = card->pos();
    QPointF end = QPointF(start.x(), -50);
    animateDeal(card, start, end);
}

void GamePage::CardSelected(CardItem *card)
{
    qDebug() << "++++++Num in selected:" << card->getNum();
    QPointF start;
    QPointF end;
    if(!m_cards.empty()){
    auto lastCard = m_cards.back();
    start = card->pos();
    end = QPointF(lastCard->x()+50, 100);
    m_cards.push_back(card);
    }
    else{
        m_cards.push_back(card);
        start = card->pos();
        end = QPointF(0, 100);
    }
    auto itr = std::find(m_visibleCards.begin(),m_visibleCards.end(),card);
    if(itr == m_visibleCards.end()){
        qDebug() << "Reached end of visible cards---------";
    }

    if (m_cardTimelines.contains(card)) {
        qDebug() << "deleting animation";

        QTimeLine* oldTl = m_cardTimelines.value(card);
        if (oldTl->state() == QTimeLine::Running)
            oldTl->stop();
        oldTl->deleteLater();
        m_cardTimelines.remove(card);
    }

    qDebug() << "removing items form the scene";
    _scene->removeItem(card); //potential pitfall
    m_cards.back()->setPos(start);
    _scene->addItem(m_cards.back());
    animateDeal(m_cards.back(), start, end);

    for(auto card:m_visibleCards){
        if (m_cardTimelines.contains(card)) {
            QTimeLine* oldTl = m_cardTimelines.value(card);
            if (oldTl->state() == QTimeLine::Running)
                oldTl->stop();
            oldTl->deleteLater();
            m_cardTimelines.remove(card);
        }
        if(std::find(m_cards.begin(),m_cards.end(),card) != m_cards.end()) continue;
        animateDeal(card,card->pos(),QPointF(card->pos().x(),10000),10000);
        card->setFlag(QGraphicsItem::ItemIsSelectable, false);
        card->disconnect(card, &CardItem::doubleClicked, this,  &GamePage::CardSelected);
        //_scene->removeItem(card);
    }
    m_cards.back()->setFlag(QGraphicsItem::ItemIsSelectable, false);
    m_cards.back()->disconnect(m_cards.back(), &CardItem::doubleClicked, this,  &GamePage::CardSelected);
    qDebug() << "end of card select but after show main crash";

    ShowMainCards(50);
}

void GamePage::animateDeal(CardItem *card, const QPointF &startPos, const QPointF &endPos,const int& Time)
{
    QTimeLine *timeLine = new QTimeLine(Time, this); //250 ms
    timeLine->setFrameRange(0, 100);

    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    animation->setItem(card);
    animation->setTimeLine(timeLine);

    // Interpolate from startPos -> endPos over 0..100
    for (int i = 0; i <= 100; ++i) {
        qreal t = i / 100.0;
        QPointF pos = startPos * (1.0 - t) + endPos * t;
        animation->setPosAt(i / 100.0, pos);
    }
    qDebug() << "animation crash";

    connect(timeLine, &QTimeLine::finished, this,[timeLine, animation]() {
        //Clean up when done
        animation->deleteLater();
        timeLine->deleteLater();
    });

    m_cardTimelines[card] = timeLine;

    connect(timeLine, &QTimeLine::finished, this, [this, timeLine, animation, card]() {
        animation->deleteLater();
        timeLine->deleteLater();
        // As soon as this timeline is done, erase it from the map
        m_cardTimelines.remove(card);
    });
    timeLine->start();

}

void GamePage::SessionOrders(const QJsonDocument &doc)
{
    qDebug() << "session";
    stopAllAnimations();

    QJsonObject obj = doc.object();
    auto cmd = obj["cmd"].toString();
    CardItem* newCard = nullptr;

    if(cmd != "ALMOST_TIMEOUT" && cmd != "TIMEOUT"){
        qDebug() << "checking oldcards in visibles crash";



        for (CardItem* oldCard : m_visibleCards) {
            if(std::find(m_cards.begin(),m_cards.end(),oldCard) != m_cards.end()){
                continue;}
            if(oldCard->scene() == _scene)
                _scene->removeItem(oldCard);
            delete oldCard;
        }
        m_visibleCards.clear();
        ShowMainCards(50);
    }
    qDebug() << "delete crash";



    if(cmd == "PLAYERS_ORDER"){

        int yours = obj["yours"].toInt();
        int opponents = obj["opponents"].toInt();
        newCard = new CardItem(m_facePixmaps[yours-1]);
        m_visibleCards.push_back(newCard);
        newCard = new CardItem(m_facePixmaps[opponents-1]);
        m_visibleCards.push_back(newCard);
        ShowCards();
        showFadingMessage("You are "+obj["result"].toString());
        for(auto card:m_visibleCards){
            animateDeal(card,card->pos(),QPointF(card->pos().x(),1000),10000);
        }
        qDebug() << "order crash";

        return;

    }
    else if(cmd == "CARD_BATCH"){

        qDebug() << "start batch crash";

        for(int i=0;;i++){
            QString key = QString::number(i);
            if (!obj.contains(key))
                break;
            int n = obj[key].toInt();
            newCard = new CardItem(m_facePixmaps[n-1],nullptr,n);
            SetupCardConnections(newCard);
            m_visibleCards.push_back(newCard);
        }
        ShowCards(75,0);
        qDebug() << "Batch crash";
        return;

    }
    else if(cmd == "ALMOST_TIMEOUT"){
        showFadingMessage("10 Seconds remaining...",1000,5000,1000);
        return;
    }
    else if(cmd == "TIMEOUT"){
        m_cards.push_back(m_visibleCards.back());
        m_visibleCards.pop_back();
        qDebug() << "TIMEOUT crash";
        for(auto card:m_visibleCards){
            animateDeal(card,card->pos(),QPointF(card->pos().x(),1000),1000);

        }
        return;
    }
    else if(cmd == "ROUND_RESULT"){
        showFadingMessage("You got:"+obj[_player->username()].toString()+"\nYour opponent:"+obj[opponent].toString(),1000,5000,1000);
        for(auto& card:m_cards){
            if(card->scene() == _scene)
                _scene->removeItem(card);
            delete card;
        }
        m_cards.clear();
    }

    //ShowCards();


}






