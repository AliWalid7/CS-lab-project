#include "Message.h"

QJsonObject Message::toJson() const {
    QJsonObject json;
    json["type"] = type;
    json["username"] = username;
    json["text"] = text;
    return json;
}

Message Message::fromJson(const QJsonObject& json) {
    Message msg;
    msg.type = json["type"].toString();
    msg.username = json["username"].toString();
    msg.text = json["text"].toString();
    return msg;
}