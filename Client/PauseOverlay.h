#ifndef PAUSEOVERLAY_H
#define PAUSEOVERLAY_H

#include <QObject>
#include "qbrush.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QFont>

class PauseOverlay : public QGraphicsRectItem {
public:
    PauseOverlay(const QRectF& sceneRect, const QString str="Paused")
        : QGraphicsRectItem(sceneRect)
    {
        // Solid black overlay
        setBrush(QBrush(Qt::black));

        // Set Z-value high to ensure overlay is on top
        setZValue(999);

        // Accept all mouse events
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::AllButtons);

        // Create and configure the "Paused" message
        QGraphicsTextItem* pauseText = new QGraphicsTextItem(str, this);
        pauseText->setDefaultTextColor(Qt::white);
        QFont font("Arial", 24, QFont::Bold);
        pauseText->setFont(font);

        // Center the text within the overlay
        QRectF textRect = pauseText->boundingRect();
        pauseText->setPos(
            (sceneRect.width() - textRect.width()) / 2,
            (sceneRect.height() - textRect.height()) / 2
            );
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* ev) override { ev->accept(); }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) override { ev->accept(); }
    void hoverMoveEvent(QGraphicsSceneHoverEvent* ev) override { ev->accept(); }
};




#endif // PAUSEOVERLAY_H
