#include "CardItem.h"



CardItem::CardItem(const QPixmap &facePixmap, const QPixmap &backPixmap, QGraphicsItem *parent):
    QObject(),QGraphicsPixmapItem(backPixmap,parent),m_face(facePixmap),m_back(backPixmap),m_faceUp(false)
{
    setPixmap(m_face);

    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void CardItem::setFaceUp(bool up)
{
    m_faceUp = up;
    setPixmap(m_faceUp? m_face:m_back);
}

void CardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(this);
    QGraphicsPixmapItem::mousePressEvent(event);
}

void CardItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit doubleClicked(this);
    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
}

