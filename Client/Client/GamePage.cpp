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


    if (!m_backPixmap.load(":/Images/cards/back.png")) {
        qWarning() << "Failed to load back.png";
    }

    //Preload all 52 face‐side pixmaps into m_facePixmaps
    for (int i = 0; i < 52; ++i) {
        int n = i + 1;
        int suitIdx, rankIdx;
        decodeCardIndex(n, suitIdx, rankIdx);

        // Build a resource path like ":/cards/Coin-2.JPG"
        QString path = QString(":/Images/cards/%2-%1.JPG")
                           .arg(RANK_NAMES[rankIdx])
                           .arg(SUIT_NAMES[suitIdx]);

        QPixmap px;
        if (!px.load(path)) {
            qWarning() << "Could not load card image at" << path;
        }
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

void GamePage::ShowCards()
{
    int i =0;
    for(auto& card:m_visibleCards){
        connect(card, &CardItem::clicked, this, &GamePage::onCardClicked);
        connect(card, &CardItem::doubleClicked, this,  &GamePage::CardSelected);
        card->setPos(i,0);
        _scene->addItem(card);
        i+=1000;
    }
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
    QPointF start = card->pos();
    QPointF end = QPointF(start.x(), 500);
    animateDeal(card, start, end);
}

void GamePage::animateDeal(CardItem *card, const QPointF &startPos, const QPointF &endPos)
{
    QTimeLine *timeLine = new QTimeLine(250, this); //250 ms
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

    connect(timeLine, &QTimeLine::finished, this,[timeLine, animation]() {
        //Clean up when done
        animation->deleteLater();
        timeLine->deleteLater();
    });

    timeLine->start();
}

void GamePage::SessionOrders(const QJsonDocument &doc)
{
    for (CardItem* oldCard : m_visibleCards) {
        delete oldCard;
    }
    m_visibleCards.clear();

    QJsonObject obj = doc.object();
    auto cmd = obj["cmd"].toString();
    CardItem* newCard = nullptr;
    if(cmd == "PLAYERS_ORDER"){
        for(int i=1;i<=2;i++){
            int yours = obj["yours"].toInt();
            int opponents = obj["opponents"].toInt();
            newCard = new CardItem(m_facePixmaps[yours],m_backPixmap);
            m_visibleCards.push_back(newCard);
            newCard = new CardItem(m_facePixmaps[opponents],m_backPixmap);
            m_visibleCards.push_back(newCard);
            ShowCards();

            QPointF start = m_visibleCards[0]->pos();
            QPointF end = QPointF(start.x(), 50000);
            animateDeal(m_visibleCards[0], start, end);

            start = m_visibleCards[1]->pos();
            end = QPointF(start.x(), -50000);
            animateDeal(m_visibleCards[1], start, end);

        }
    }
    else if(cmd == "PLAYER1_CARDS_SENT"){
        for(int i=1;i<=7;i++){
            int n = obj[QString::number(i)].toInt();
            newCard = new CardItem(m_facePixmaps[n],m_backPixmap);

            m_visibleCards.push_back(newCard);
        }
    }
    else if(cmd == "PLAYER2_CARDS_SENT"){
        for(int i=1;i<=6;i++){
            int n = obj[QString::number(i)].toInt();
            newCard = new CardItem(m_facePixmaps[n],m_backPixmap);

            m_visibleCards.push_back(newCard);
        }
    }
    //ShowCards();


}






