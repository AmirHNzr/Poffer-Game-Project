#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>




class CardItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    CardItem(const QPixmap &facePixmap, const QPixmap &backPixmap, QGraphicsItem *parent = nullptr);

    void setFaceUp(bool up);

    bool isFaceUp() const { return m_faceUp; }

signals:
    void clicked(CardItem *self);
    void doubleClicked(CardItem* self);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;  // ← override

private:
    QPixmap m_face;
    QPixmap m_back;
    bool   m_faceUp;
};


#endif // CARDITEM_H
