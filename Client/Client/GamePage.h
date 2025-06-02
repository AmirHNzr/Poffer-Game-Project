#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QDialog>
#include "CardItem.h"
#include"PlayerInfo.h"
#include "qgraphicsitem.h"
#include<QJsonArray>
#include <QGraphicsView>
#include"UserController.h"

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
    QJsonArray _players;
    UserController* _controller;
    QVector<CardItem*> m_cards;

    // Holds all 52 “face” pixmaps
    std::array<QPixmap, 52> m_facePixmaps;

    // One single back‐image pixmap
    QPixmap m_backPixmap;

    //keep pointers of dealt card (one for each visible CardItem)
    std::vector<CardItem*> m_visibleCards;

    QGraphicsView*  m_view;
protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    //Slot when any CardItem is clicked
    void onCardClicked(CardItem *card);

    //Slot when a card is double clicked
    void CardSelected(CardItem* card);

    //deal (move) a card from one point to another
    void animateDeal(CardItem *card, const QPointF &startPos, const QPointF &endPos);

    void SessionOrders(const QJsonDocument&);
};

#endif // GAMEPAGE_H
