#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "Message.h"

class NetworkClient : public QObject {
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, int port);
    void disconnectFromServer();
    void sendMessage(const Message &msg);
    void sendJoinRequest(const QString &username);
    void sendChatMessage(const QString &username, const QString &text);

signals:
    void connected();
    void disconnected();
    void errorOccurred(QString error);
    void messageReceived(Message msg);
    void joinSuccess();
    void userListReceived(QStringList users);
    void userLeft(QString username);

private:
    QTcpSocket *socket;
    void setupConnections();
    void sendJsonMessage(const QJsonObject &json);
};

#endif