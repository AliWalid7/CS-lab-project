#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <QObject>
#include <QStringList>

class ChatController : public QObject
{
    Q_OBJECT

public:
    explicit ChatController(QObject *parent = nullptr);

    QStringList onlineUsers() const;
    bool addOnlineUser(const QString &name);
    bool removeOnlineUser(const QString &name);
    void clearOnlineUsers();

public slots:
    void setOnlineUsers(const QStringList &users);

signals:
    void onlineUsersChanged(const QStringList &users);

private:
    bool isValidUserName(const QString &name) const;

    QStringList m_users;
};

#endif
