#include "MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    appState = new AppState(this);
    authManager = new AuthManager(this);
    chatManager = new ChatManager(this);
    networkClient = new JsonNetworkClient(this);
    
    setupUi();

    connect(appState, &AppState::screenChanged, this, &MainWindow::switchToScreen);
    
    connect(loginWindow, &LoginWindow::loginRequested,this,&MainWindow::handleLoginRequested);
    
    connect(authManager, &AuthManager::loginSucceeded,this, &MainWindow::onLoginSucceeded);
    
    connect(authManager, &AuthManager::loginFailed,this, &MainWindow::onLoginFailed);
    
    connect(chatWindow, &ChatWindow::sendMessageRequested,this, &MainWindow::handleSendMessageRequested);
    
    connect(chatWindow, &ChatWindow::logoutRequested, this, &MainWindow::handleLogoutRequested);
    
    connect(networkClient, &JsonNetworkClient::connected,this, &MainWindow::onConnected);
    
    connect(networkClient, &JsonNetworkClient::disconnected,this, &MainWindow::onDisconnected);
    
    connect(networkClient, &JsonNetworkClient::messageReceived,this, &MainWindow::onMessageReceived);
    
    connect(networkClient, &JsonNetworkClient::userListReceived,this, &MainWindow::onUserListReceived);
    
    appState->setCurrentScreen(AppScreens::Login);
    
}
MainWindow::~MainWindow()
{
    delete stackedWidget;
}

void MainWindow::setupUi()
{
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    loginWindow = new LoginWindow(this);
    chatWindow = new ChatWindow(this);

    stackedWidget->addWidget(loginWindow);
    stackedWidget->addWidget(chatWindow);

    setWindowTitle("CECE Chat");
    resize(900, 650);
}

void MainWindow::switchToScreen(const QString &screenName)
{
    if (screenName == AppScreens::Login) {
        stackedWidget->setCurrentWidget(loginWindow);
    }
    else if (screenName == AppScreens::Chat) {
        stackedWidget->setCurrentWidget(chatWindow);
    }
}

void MainWindow::handleLoginRequest(const QString &username, const QString &password)
{
    currentUsername = username;
    authManager->attemptLogin(username, password);
}
void MainWindow::onLoginSucceeded(const QString &username)
{
    currentUsername = username;
    loginWindow->clearFields();

    // Connecttoserver
    networkClient->connectToServer("127.0.0.1", NetworkDefaults::Port);

    appState->setCurrentScreen(AppScreens::Chat);
}

void MainWindow::onLoginFailed(const QString &reason)
{
    loginWindow->showError(reason);
}

void MainWindow::handleSendMessageRequested(const QString &message)
{
    if (networkClient && appState->isConnected()) {
        networkClient->sendChatMessage(currentUsername, message);
        chatManager->addMessage(currentUsername, message);
        chatWindow->addMessage(currentUsername, message, true);
    }
}

void MainWindow::handleLogoutRequested()
{
    networkClient->disconnectFromServer();
    chatManager->clearHistory();
    chatManager->clearUsers();

    authManager->logout();
    appState->setCurrentScreen(AppScreens::Login);
}

void MainWindow::onConnected()
{
    // Join the chat room
    networkClient->sendJoinRequest(currentUsername);
    appState->setConnected(true);
    chatWindow->showConnectionStatus("Connected", true);
}

void MainWindow::onDisconnected()
{
    appState->setConnected(false);
    chatWindow->showConnectionStatus("Disconnected", false);

    if (appState->currentScreen() == AppScreens::Chat) {
        QMessageBox::warning(this, "Connection Lost", "Disconnected from server.");
        handleLogoutRequested();
    }
}

void MainWindow::onMessageReceived(const Message &msg)
{
    if (msg.type == "broadcast") {
        bool isMine = (msg.username == currentUsername);
        chatWindow->addMessage(msg.username, msg.text, isMine);
        chatManager->addMessage(msg.username, msg.text);
    }
}

void MainWindow::onUserListReceived(const QStringList &users)
{
    chatWindow->updateUserList(users);
}
