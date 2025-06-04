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

    if (!m_backPixmap.load(":/Images/cards/back.png")) {
        qWarning() << "Failed to load back.png";
    }

    QSize targetSize(100, 100);
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
        px = px.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
    int x =0;
    int y=0;
    for(auto& card:m_visibleCards){
        connect(card, &CardItem::clicked, this, &GamePage::onCardClicked);
        connect(card, &CardItem::doubleClicked, this,  &GamePage::CardSelected);

        card->setPos(x,y);
        _scene->addItem(card);
        x+=xOffset;
        y-=yOffset;
    }
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
    QPointF start = card->pos();
    QPointF end = QPointF(start.x(), 500);
    animateDeal(card, start, end);
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

        int yours = obj["yours"].toInt();
        int opponents = obj["opponents"].toInt();
        newCard = new CardItem(m_facePixmaps[yours],m_backPixmap);
        m_visibleCards.push_back(newCard);
        newCard = new CardItem(m_facePixmaps[opponents],m_backPixmap);
        m_visibleCards.push_back(newCard);
        ShowCards();
        showFadingMessage("You are "+obj["result"].toString());
        for(auto card:m_visibleCards)
            animateDeal(card,card->pos(),QPointF(card->pos().x(),10000),5000);

        return;

    }
    else if(cmd == "CARD_BATCH"){
        for(int i=0;;i++){
            QString key = QString::number(i);
            if (!obj.contains(key))
                break;
            int n = obj[key].toInt();
            newCard = new CardItem(m_facePixmaps[n],m_backPixmap);

            m_visibleCards.push_back(newCard);
        }
        ShowCards(50,0);
    }

    //ShowCards();


}






