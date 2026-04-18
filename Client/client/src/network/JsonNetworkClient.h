#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "message.h"

class NetworkClient : public QObject {
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, int port);
    void sendMessage(const Message &msg);
    void sendJoinRequest(const QString &username);
    void sendChatMessage(const QString &username, const QString &text);

signals:
    void connected();
    void disconnected();
    void errorOccurred(QString error);
    void messageReceived(Message msg);

private:
    QTcpSocket *socket;
    void setupConnections();
    void sendJsonMessage(const QJsonObject &json);
};

#endif