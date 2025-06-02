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
{
    ui->setupUi(this);


    if (!m_backPixmap.load(":/cards/back.png")) {
        qWarning() << "Failed to load back.png";
    }

    //Preload all 52 face‐side pixmaps into m_facePixmaps
    for (int i = 0; i < 52; ++i) {
        int n = i + 1;
        int suitIdx, rankIdx;
        decodeCardIndex(n, suitIdx, rankIdx);

        // Build a resource path like ":/cards/Coin-2.JPG"
        QString path = QString(":/cards/%2-%1.JPG")
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

void GamePage::SessionOrders(const QJsonDocument &doc)
{
    for (CardItem* oldCard : m_visibleCards) {
        delete oldCard;
    }
    m_visibleCards.clear();

    QJsonObject obj = doc.object();
    auto cmd = obj["cmd"].toString();
    CardItem* newCard = nullptr;
    if(cmd == "PLAYER1_CARDS_SENT"){
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


}






