#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>

struct QueueEntry {
    QString username;
    QTcpSocket* socket;
};

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);
    //Add players to queue
    void enqueuePlayer(const QString& username, QTcpSocket* sock);

    void broadcastQueueCount();




    std::vector<QueueEntry> sessionPlayers() const;

signals:
    void queueUpdated(int count);


private:
    std::vector<QueueEntry> m_queue;
    std::vector<QueueEntry> _sessionPlayers;
    static constexpr int REQUIRED_PLAYERS = 2;

    void tryStartGame();

};

#endif // GAMEMANAGER_H
