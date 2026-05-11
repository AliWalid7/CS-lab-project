#include "ClientController.h"

#include "AppState.h"
#include "AuthManager.h"
#include "ChatManager.h"
#include "JsonNetworkClient.h"

ClientController::ClientController(QObject *parent)
    : QObject(parent)
{
    appState = new AppState(this);
    authManager = new AuthManager(this);
    chatManager = new ChatManager(this);
    networkClient = new NetworkClient(this);

    connect(appState, &AppState::screenChanged, this, &ClientController::screenChanged);
    connect(authManager, &AuthManager::loginFailed, this, [this](const QString &reason) {
        emit loginLoadingChanged(false);
        emit loginError(reason);
    });
    connect(networkClient, &NetworkClient::connected, this, &ClientController::onConnected);
    connect(networkClient, &NetworkClient::disconnected, this, &ClientController::onDisconnected);
    connect(networkClient, &NetworkClient::joinSuccess, this, &ClientController::onJoinSuccess);
    connect(networkClient, &NetworkClient::messageReceived, this, &ClientController::onMessageReceived);
    connect(networkClient, &NetworkClient::userListReceived, this, &ClientController::onUserListReceived);
    connect(networkClient, &NetworkClient::errorOccurred, this, &ClientController::onNetworkError);

    appState->setCurrentScreen("Login");
}

void ClientController::login(const QString &username, const QString &password)
{
    emit loginLoadingChanged(true);

    if (!authManager->attemptLogin(username, password)) {
        emit loginLoadingChanged(false);
        return;
    }

    currentUsername = username.trimmed();
    loginInProgress = true;
    joinedChat = false;
    loggingOut = false;
    networkClient->connectToServer("127.0.0.1", 54321);
}

void ClientController::sendMessage(const QString &message)
{
    if (!appState->isConnected() || currentUsername.isEmpty()) {
        return;
    }

    networkClient->sendChatMessage(currentUsername, message);
}

void ClientController::logout()
{
    loggingOut = true;

    if (appState->isConnected()) {
        networkClient->disconnectFromServer();
        return;
    }

    resetSession();
    loggingOut = false;
}

void ClientController::onConnected()
{
    appState->setConnected(true);
    emit connectionStatusChanged("Connected", true);

    if (!currentUsername.isEmpty()) {
        networkClient->sendJoinRequest(currentUsername);
    }
}

void ClientController::onDisconnected()
{
    const bool wasActive = loginInProgress || joinedChat || appState->currentScreen() == "Chat";
    const bool shouldShowError = wasActive && !loggingOut;

    loginInProgress = false;
    joinedChat = false;
    appState->setConnected(false);
    emit connectionStatusChanged("Disconnected", false);

    if (wasActive) {
        resetSession();
    }

    loggingOut = false;

    if (shouldShowError) {
        emit loginError("Disconnected from server.");
    }
}

void ClientController::onJoinSuccess()
{
    loginInProgress = false;
    joinedChat = true;
    authManager->onServerLoginResponse(true, {});
    emit loginLoadingChanged(false);
    emit loginSucceeded();
    appState->setCurrentScreen("Chat");
}

void ClientController::onMessageReceived(const Message &message)
{
    if (message.type != "broadcast") {
        return;
    }

    chatManager->addMessage(message.username, message.text);
    emit messageAdded(message.username, message.text, message.username == currentUsername);
}

void ClientController::onUserListReceived(const QStringList &users)
{
    emit userListChanged(users);
}

void ClientController::onNetworkError(const QString &error)
{
    emit connectionStatusChanged(error, false);

    if (!loginInProgress) {
        return;
    }

    loginInProgress = false;
    currentUsername.clear();
    authManager->onServerLoginResponse(false, error);
    networkClient->disconnectFromServer();
}

void ClientController::resetSession()
{
    chatManager->clearHistory();
    chatManager->clearUsers();
    authManager->logout();
    currentUsername.clear();
    emit chatCleared();
    emit userListChanged({});
    appState->setCurrentScreen("Login");
}
