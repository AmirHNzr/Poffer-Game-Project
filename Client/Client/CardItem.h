#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>




class CardItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    CardItem(const QPixmap &facePixmap, QGraphicsItem *parent = nullptr,const int& num=0);


    QPixmap face() const;

    int getNum() const;
    void setNum(int newNum);

    bool operator==(const CardItem& lhs);

signals:
    void doubleClicked(CardItem* self);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPixmap m_face;
    int num;
};


#endif // CARDITEM_H
