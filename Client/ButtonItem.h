#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

class ButtonItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    //defaultPix  the pixmap to show normally
    //hoverPix    the pixmap to show when hovered
    explicit ButtonItem(const QPixmap& defaultPix,
                        const QPixmap& hoverPix,
                        QGraphicsItem* parent = nullptr)
        : QGraphicsPixmapItem(defaultPix, parent)
        , m_default(defaultPix)
        , m_hover(hoverPix)
    {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::LeftButton);
        setCursor(QCursor(Qt::PointingHandCursor));
    }

signals:
    void clicked();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* ev) override {
        setPixmap(m_hover);
        QGraphicsPixmapItem::hoverEnterEvent(ev);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* ev) override {
        setPixmap(m_default);
        QGraphicsPixmapItem::hoverLeaveEvent(ev);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* ev) override {
        setOffset(2,2);
        QGraphicsPixmapItem::mousePressEvent(ev);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) override {
        setOffset(0,0);
        QPointF localPos = ev->pos() - offset();
        if (contains(localPos)) {
            emit clicked();
        }
        QGraphicsPixmapItem::mouseReleaseEvent(ev);
    }

private:
    QPixmap m_default;
    QPixmap m_hover;
};

#endif // BUTTONITEM_H
