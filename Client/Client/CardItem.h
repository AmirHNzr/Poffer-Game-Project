#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>

class CardItem : public QObject
{
    Q_OBJECT
public:
    explicit CardItem(QObject *parent = nullptr);

signals:
};

#endif // CARDITEM_H
