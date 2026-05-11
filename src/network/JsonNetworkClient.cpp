#include "JsonNetworkClient.h"
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);
    setupConnections();
}

void NetworkClient::connectToServer(const QString &host, int port) {
    socket->connectToHost(host, port);
}

void NetworkClient::disconnectFromServer() {
    socket->disconnectFromHost();
}

void NetworkClient::sendMessage(const Message &msg) {
    QJsonDocument doc(msg.toJson());
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    data.append('\n');
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
    data.append('\n');
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
        receiveBuffer.append(socket->readAll());

        while (true) {
            int newlineIndex = receiveBuffer.indexOf('\n');
            if (newlineIndex < 0) {
                break;
            }

            QByteArray line = receiveBuffer.left(newlineIndex);
            receiveBuffer.remove(0, newlineIndex + 1);

            if (!line.isEmpty() && line.endsWith('\r')) {
                line.chop(1);
            }

            if (line.trimmed().isEmpty()) {
                continue;
            }

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(line, &parseError);

            if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
                emit errorOccurred("Invalid JSON");
                continue;
            }

            QJsonObject json = doc.object();
            QString type = json["type"].toString();

            if (type == "join_success") {
                emit joinSuccess();
            }
            else if (type == "broadcast") {
                Message msg;
                msg.type = type;
                msg.username = json["username"].toString();
                msg.text = json["text"].toString();
                emit messageReceived(msg);
            }
            else if (type == "error") {
                QString errorMsg = json["message"].toString();
                emit errorOccurred(errorMsg);
            }
            else if (type == "user_list") {
                QStringList users;
                QJsonArray usersArray = json["users"].toArray();
                for (const auto& user : usersArray) {
                    users.append(user.toString());
                }
                emit userListReceived(users);
            }
            else if (type == "user_left") {
                QString username = json["username"].toString();
                emit userLeft(username);
            }
        }
    });
}
