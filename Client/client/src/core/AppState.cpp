#include "AppState.h"

AppState::AppState(QObject *parent) : QObject(parent), m_currentScreen("login"), m_connected(false) {}

QString AppState::currentScreen() const
{
    return m_currentScreen;
}

void AppState::setCurrentScreen(const QString& screen)
{
    m_currentScreen = screen;
    emit screenChanged(screen);
}

bool AppState::isConnected() const
{
    return m_connected;
}

void AppState::setConnected(bool status)
{
    m_connected = status;
    emit connectionChanged(status);
}