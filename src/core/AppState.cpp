#include "AppState.h"

#include "types.h"

AppState::AppState(QObject *parent)
    : QObject(parent), m_currentScreen(AppScreens::Login), m_connected(false)
{
}

QString AppState::currentScreen() const
{
    return m_currentScreen;
}

void AppState::setCurrentScreen(const QString& screen)
{
    if (m_currentScreen == screen)
        return;

    m_currentScreen = screen;
    emit screenChanged(screen);
}

bool AppState::isConnected() const
{
    return m_connected;
}

void AppState::setConnected(bool status)
{
    if (m_connected == status)
        return;

    m_connected = status;
    emit connectionChanged(status);
}
