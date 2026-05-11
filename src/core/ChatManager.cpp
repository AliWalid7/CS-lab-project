#include "ChatManager.h"
#include "DatabaseManager.h"

ChatManager::ChatManager(QObject *parent) : QObject(parent) {
    DatabaseManager db;
    db.openDatabase();
}

bool ChatManager::addMessage(const QString& username, const QString& text)
{
    if (username.isEmpty() || text.isEmpty())
        return false;
    Message msg;
    msg.username = username;
    msg.text = text;
    m_history.append(msg);
    DatabaseManager db;
    db.saveMessage(
    username.toStdString(),
    "",
    text.toStdString()
);
    emit historyChanged(m_history);
    return true;
}

QList<Message> ChatManager::getHistory() const
{
    return m_history;
}

void ChatManager::clearHistory()
{
    m_history.clear();
    emit historyChanged(m_history);
}

bool ChatManager::addUser(const QString& username)
{
    if (username.isEmpty())
        return false;
    QString lower = username.toLower();
    for (const QString& u : m_users)
        if (u.toLower() == lower)
            return false;
    m_users.append(username);
    emit usersChanged(m_users);
    return true;
}

bool ChatManager::removeUser(const QString& username)
{
    QString lower = username.toLower();
    for (int i = 0; i < m_users.size(); i++) {
        if (m_users[i].toLower() == lower) {
            m_users.removeAt(i);
            emit usersChanged(m_users);
            return true;
        }
    }
    return false;
}

QStringList ChatManager::getUsers() const
{
    return m_users;
}

void ChatManager::clearUsers()
{
    m_users.clear();
    emit usersChanged(m_users);
}
