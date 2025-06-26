#include "GameManager.h"

GameManager::GameManager(QObject *parent)
    : QObject{parent}
{}

void GameManager::enqueuePlayer(const QString &username, QTcpSocket *sock)
{
    QueueEntry* queue = new QueueEntry(username,sock);
    qDebug() << ">><< New entry:" << username + " - "<< sock->socketDescriptor();
    m_queue.push_back(queue);

    // Check if we have enough players now:
    tryStartGame();
}

// std::vector<QueueEntry> GameManager::sessionPlayers() const
// {
//     return _sessionPlayers;
// }

void GameManager::tryStartGame()
{
    if ((int)m_queue.size() < REQUIRED_PLAYERS)
        return;  // not enough yet

    QJsonArray playerArray;
    for (int i = 0; i < REQUIRED_PLAYERS; ++i) {
        playerArray.append(m_queue[i]->username);
    }

    //Build the GAME_READY JSON for those players:
    QJsonObject readyObj;
    readyObj["cmd"]     = "GAME_READY";
    readyObj["players"] = playerArray;

    QJsonDocument doc(readyObj);
    QByteArray   bytes = doc.toJson(QJsonDocument::Compact) + "\n";

    //Send “GAME_READY” to each socket:
    for (int i = 0; i < REQUIRED_PLAYERS; ++i) {
        _sessionPlayers.push_back(m_queue[i]);
        QTcpSocket* s = m_queue[i]->socket;
        qDebug() << "{}{}{} Sending to:" << s->socketDescriptor();
        if (s && s->state() == QAbstractSocket::ConnectedState) {
            s->write(bytes);
            s->flush();
        }
    }

    //Remove those first N entries from the queue:
    //m_queue.erase(m_queue.begin(), m_queue.begin() + REQUIRED_PLAYERS);

}
