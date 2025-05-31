#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(QObject *parent)
    : QObject{parent},_username{""}
{}

QString PlayerInfo::username() const
{
    return _username;
}

void PlayerInfo::setUsername(const QString &newUsername)
{
    _username = newUsername;
}
