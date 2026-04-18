#include "chatmanager.h"

#include <QJsonDocument>
#include <QJsonObject>

ChatManager::ChatManager() {}

QByteArray ChatManager::joinMessage(const QString &username)
{
    QJsonObject obj;
    obj["type"] = "join";
    obj["username"] = username;
    return QJsonDocument(obj).toJson();
}

QByteArray ChatManager::chatMessage(const QString &username, const QString &text)
{
    QJsonObject obj;
    obj["type"] = "message";
    obj["username"] = username;
    obj["text"] = text;
    return QJsonDocument(obj).toJson();
}

void ChatManager::addToHistory(const QString &username, const QString &text)
{
    Message msg;
    msg.username = username;
    msg.text = text;
    m_history.append(msg);
}

QList<Message> ChatManager::getHistory()
{
    return m_history;
}

void ChatManager::clearHistory()
{
    m_history.clear();
}
