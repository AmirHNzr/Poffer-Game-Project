#include "CardItem.h"



CardItem::CardItem(const QPixmap &facePixmap, QGraphicsItem *parent,const int& num):
    QObject(),QGraphicsPixmapItem(facePixmap,parent),m_face(facePixmap),num{num}
{
    setPixmap(m_face);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void CardItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit doubleClicked(this);
    QGraphicsPixmapItem::mouseDoubleClickEvent(event);
}

int CardItem::getNum() const
{
    return num;
}

void CardItem::setNum(int newNum)
{
    num = newNum;
}

bool CardItem::operator==(const CardItem &lhs)
{
    return this->num == lhs.num;
}

QPixmap CardItem::face() const
{
    return m_face;
}

