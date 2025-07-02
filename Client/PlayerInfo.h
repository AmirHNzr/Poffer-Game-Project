#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QObject>

class PlayerInfo : public QObject
{
    Q_OBJECT
public:
    explicit PlayerInfo(QObject *parent = nullptr);

    QString username() const;
    void setUsername(const QString &newUsername);

    void JsonToHistory(const QJsonObject& obj);

signals:

private:
    QString _username;

};

#endif // PLAYERINFO_H
