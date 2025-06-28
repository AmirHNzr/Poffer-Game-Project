#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>

struct QueueEntry {
    QueueEntry(QString u,QTcpSocket* s){username = u;socket=s;wins=0;}
    QueueEntry(){wins = 0;}
    QString username;
    QTcpSocket* socket;
    int wins;
};

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);
    //Add players to queue
    void enqueuePlayer(const QString& username, QTcpSocket* sock);

    void broadcastQueueCount();
    std::vector<QueueEntry*> _sessionPlayers;
    std::vector<QueueEntry *> queue() const;
    void setQueueZero();

signals:
    void queueUpdated(int count);
private:
    std::vector<QueueEntry*> m_queue;
    static constexpr int REQUIRED_PLAYERS = 2;

    void tryStartGame();

};

#endif // GAMEMANAGER_H
