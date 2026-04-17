#include "chatcontroller.h"

ChatController::ChatController(QObject *parent)
    : QObject(parent)
{
}

QStringList ChatController::onlineUsers() const
{
    return m_users;
}

bool ChatController::addOnlineUser(const QString &name)
{
    if (name.isEmpty())
        return false;

    QString lowerName = name.toLower();

    for (int i = 0; i < m_users.size(); i++) {
        if (m_users[i].toLower() == lowerName)
            return false;
    }

    m_users.append(name);

    onlineUsersChanged(m_users);

    return true;
}

bool ChatController::removeOnlineUser(const QString &name)
{
    QString lowerName = name.toLower();

    for (int i = 0; i < m_users.size(); i++) {
        if (m_users[i].toLower() == lowerName) {
            m_users.removeAt(i);

            onlineUsersChanged(m_users);

            return true;
        }
    }

    return false;
}

void ChatController::clearOnlineUsers()
{
    m_users.clear();

    onlineUsersChanged(m_users);
}
