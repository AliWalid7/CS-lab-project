#pragma once
#include <QString>
#include <QList>
#include <QObject>

struct Message {
    QString username;
    QString text;
};

class ChatManager : public QObject
{
    Q_OBJECT
public:
    ChatManager(QObject *parent = nullptr);

    bool addMessage(const QString& username, const QString& text);
    QList<Message> getHistory() const;
    void clearHistory();

    bool addUser(const QString& username);
    bool removeUser(const QString& username);
    QStringList getUsers() const;
    void clearUsers();

signals:
    void historyChanged(const QList<Message>& messages);
    void usersChanged(const QStringList& users);

private:
    QList<Message> m_history;
    QStringList m_users;
};
