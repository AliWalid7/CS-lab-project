#pragma once
#include <QString>
#include <QObject>

class AppState : public QObject
{
    Q_OBJECT
public:
    AppState(QObject *parent = nullptr);

    QString currentScreen() const;
    void setCurrentScreen(const QString& screen);

    bool isConnected() const;
    void setConnected(bool status);

signals:
    void screenChanged(const QString& screen);
    void connectionChanged(bool connected);

private:
    QString m_currentScreen;
    bool m_connected;
};
