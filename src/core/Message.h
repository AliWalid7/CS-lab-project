#pragma once
#include <QString>
#include <QJsonObject>

struct Message {
    QString type;
    QString username;
    QString text;

    QJsonObject toJson() const;
    static Message fromJson(const QJsonObject& json);
};