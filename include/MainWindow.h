#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "LoginWindow.h"
#include "ChatWindow.h"
#include "AppState.h"
#include "AuthManager.h"
#include "ChatManager.h"
#include "JsonNetworkClient.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Login
    void handleLoginRequested(const QString &username, const QString &password);
    void onLoginSucceeded(const QString &username);
    void onLoginFailed(const QString &reason);

        // Chat
    void handleSendMessageRequested(const QString &message);
    void handleLogoutRequested();

        // Network & State
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const Message &msg);
    void onUserListReceived(const QStringList &users);

private:
    void setupUi();
    void switchToScreen(const QString &screenName);
    
    QStackedWidget *stackedWidget;

        LoginWindow *loginWindow;
        ChatWindow *chatWindow;

        AppState *appState;
        AuthManager *authManager;
        ChatManager *chatManager;
        JsonNetworkClient *networkClient;

        QString currentUsername;
};

#endif
