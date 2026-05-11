#pragma once

#include <QObject>
#include <QStringList>

#include "Message.h"

class AppState;
class AuthManager;
class ChatManager;
class NetworkClient;

class ClientController : public QObject
{
    Q_OBJECT

public:
    explicit ClientController(QObject *parent = nullptr);

public slots:
    void login(const QString &username, const QString &password);
    void sendMessage(const QString &message);
    void logout();

signals:
    void screenChanged(const QString &screenName);
    void loginError(const QString &message);
    void loginLoadingChanged(bool loading);
    void loginSucceeded();
    void messageAdded(const QString &sender, const QString &message, bool isMine);
    void userListChanged(const QStringList &users);
    void connectionStatusChanged(const QString &status, bool isConnected);
    void chatCleared();

private slots:
    void onConnected();
    void onDisconnected();
    void onJoinSuccess();
    void onMessageReceived(const Message &message);
    void onUserListReceived(const QStringList &users);
    void onNetworkError(const QString &error);

private:
    void resetSession();

    AppState *appState;
    AuthManager *authManager;
    ChatManager *chatManager;
    NetworkClient *networkClient;
    QString currentUsername;
    bool loginInProgress = false;
    bool joinedChat = false;
    bool loggingOut = false;
};
