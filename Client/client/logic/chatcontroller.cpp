#include "chatcontroller.h"

ChatController::ChatController(QObject *parent)
    : QObject(parent)
{
}

QStringList ChatController::onlineUsers() const
{
    return m_users;
}

bool ChatController::isValidUserName(const QString &name) const
{
    return !name.isEmpty() && name.length() <= 32;
}

bool ChatController::addOnlineUser(const QString &name)
{
    const QString t = name.trimmed();
    if (!isValidUserName(t))
        return false;
    for (const QString &u : m_users) {
        if (u.compare(t, Qt::CaseInsensitive) == 0)
            return false;
    }
    m_users.append(t);
    m_users.sort(Qt::CaseInsensitive);
    emit onlineUsersChanged(m_users);
    return true;
}

bool ChatController::removeOnlineUser(const QString &name)
{
    const QString t = name.trimmed();
    for (int i = 0; i < m_users.size(); ++i) {
        if (m_users.at(i).compare(t, Qt::CaseInsensitive) == 0) {
            m_users.removeAt(i);
            emit onlineUsersChanged(m_users);
            return true;
        }
    }
    return false;
}

void ChatController::clearOnlineUsers()
{
    if (m_users.isEmpty())
        return;
    m_users.clear();
    emit onlineUsersChanged(m_users);
}

void ChatController::setOnlineUsers(const QStringList &users)
{
    QStringList next;
    for (const QString &raw : users) {
        const QString t = raw.trimmed();
        if (!isValidUserName(t))
            continue;
        bool dup = false;
        for (const QString &x : next) {
            if (x.compare(t, Qt::CaseInsensitive) == 0) {
                dup = true;
                break;
            }
        }
        if (!dup)
            next.append(t);
    }
    next.sort(Qt::CaseInsensitive);
    if (next == m_users)
        return;
    m_users = next;
    emit onlineUsersChanged(m_users);
}
