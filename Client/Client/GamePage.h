#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QDialog>
#include "CardItem.h"
#include"PlayerInfo.h"
#include "qgraphicsitem.h"
#include<QJsonArray>
#include <QGraphicsView>
#include"UserController.h"
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>


namespace Ui {
class GamePage;
}

class GamePage : public QDialog
{
    Q_OBJECT

public:
    explicit GamePage(UserController*,PlayerInfo*,QJsonArray,QWidget *parent = nullptr);
    ~GamePage();

private:
    Ui::GamePage *ui;
    PlayerInfo* _player;
    QString opponent;
    QJsonArray _players;
    UserController* _controller;
    std::vector<CardItem*> m_cards;
    //Controlling animations
    QHash<CardItem*, QTimeLine*>   m_cardTimelines;


    // Holds all 52 “face” pixmaps
    std::array<QPixmap, 52> m_facePixmaps;

    // One single back‐image pixmap
    QPixmap m_backPixmap;

    //keep pointers of dealt card (one for each visible CardItem)
    std::vector<CardItem*> m_visibleCards;

    QGraphicsScene *_scene;


    void ShowCards(int,int);
    void ShowMainCards(int);
    void SetupCardConnections(CardItem *);

    //Show if they are first or not
    QLabel*   m_overlayLabel;
    QGraphicsOpacityEffect* m_opacityEffect;
    QPropertyAnimation*     m_fadeAnimation;
    void stopAllAnimations();
    void SetupFadingMsg();
    void showFadingMessage(const QString& text, int fadeInMs = 500, int stayMs = 1000, int fadeOutMs = 500);
    //Fading animation

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;


private slots:
    //Slot when any CardItem is clicked
    void onCardClicked(CardItem *card);

    //Slot when a card is double clicked
    void CardSelected(CardItem* card);

    //deal (move) a card from one point to another
    void animateDeal(CardItem *card, const QPointF &startPos, const QPointF &endPos,const int& time=250);

    void SessionOrders(const QJsonDocument&);
};

#endif // GAMEPAGE_H
