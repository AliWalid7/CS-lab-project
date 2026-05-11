#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    controller = new ClientController(this);

    setupUi();

    connect(loginWindow, &LoginWindow::loginRequested, controller, &ClientController::login);
    connect(chatWindow, &ChatWindow::sendMessageRequested, controller, &ClientController::sendMessage);
    connect(chatWindow, &ChatWindow::logoutRequested, controller, &ClientController::logout);

    connect(controller, &ClientController::screenChanged, this, &MainWindow::switchToScreen);
    connect(controller, &ClientController::loginError, loginWindow, &LoginWindow::showError);
    connect(controller, &ClientController::loginLoadingChanged, loginWindow, &LoginWindow::showLoading);
    connect(controller, &ClientController::loginSucceeded, loginWindow, &LoginWindow::clearFields);
    connect(controller, &ClientController::messageAdded, chatWindow, &ChatWindow::addMessage);
    connect(controller, &ClientController::userListChanged, chatWindow, &ChatWindow::updateUserList);
    connect(controller, &ClientController::connectionStatusChanged, chatWindow, &ChatWindow::showConnectionStatus);
    connect(controller, &ClientController::chatCleared, chatWindow, &ChatWindow::clearChat);

    switchToScreen("Login");
}

MainWindow::~MainWindow()
{
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
    if (screenName == "Login") {
        stackedWidget->setCurrentWidget(loginWindow);
    }
    else if (screenName == "Chat") {
        stackedWidget->setCurrentWidget(chatWindow);
    }
}
