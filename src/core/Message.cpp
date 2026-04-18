#include "Message.h"
#include <QJsonObject>

QJsonObject Message::toJson() const {
    QJsonObject json;
    json["username"] = username;
    json["text"] = text;
    return json;
}

Message Message::fromJson(const QJsonObject& json) {
    Message msg;
    msg.username = json["username"].toString();
    msg.text = json["text"].toString();
    return msg;
}
