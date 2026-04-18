#include "JsonNetworkClient.h"
#include <QJsonDocument>
#include <QDebug>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);
    setupConnections();
}

void NetworkClient::connectToServer(const QString &host, int port) {
    socket->connectToHost(host, port);
}

void NetworkClient::sendMessage(const Message &msg) {
    QJsonDocument doc(msg.toJson());
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    socket->write(data);
}

void NetworkClient::sendJoinRequest(const QString &username) {
    QJsonObject json;
    json["type"] = "join";
    json["username"] = username;
    sendJsonMessage(json);
}

void NetworkClient::sendChatMessage(const QString &username, const QString &text) {
    QJsonObject json;
    json["type"] = "message";
    json["username"] = username;
    json["text"] = text;
    sendJsonMessage(json);
}

void NetworkClient::sendJsonMessage(const QJsonObject &json) {
    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    socket->write(data);
}

void NetworkClient::setupConnections() {
    connect(socket, &QTcpSocket::connected, this, [this]() {
        emit connected();
    });

    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        emit disconnected();
    });

    connect(socket, &QTcpSocket::errorOccurred, this, [this]() {
        emit errorOccurred(socket->errorString());
    });

    connect(socket, &QTcpSocket::readyRead, this, [this]() {
        QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (!doc.isObject()) {
            emit errorOccurred("Invalid JSON");
            return;
        }

        QJsonObject json = doc.object();
        Message msg;
        msg.username = json["username"].toString();
        msg.text = json["text"].toString();

        emit messageReceived(msg);
    });
}