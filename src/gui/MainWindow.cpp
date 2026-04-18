#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loginWindow = new LoginWindow(this);
    chatWindow = new ChatWindow(this);
    settingsWindow = new SettingsWindow(this);

    setCentralWidget(loginWindow);

    connect(loginWindow, &LoginWindow::loginRequested, this, &MainWindow::handleLoginRequest);
    connect(chatWindow, &ChatWindow::logoutRequested, this, &MainWindow::handleLogout);
}

void MainWindow::handleLoginRequest(const QString &username, const QString &password)
{
    // Forward to Business Logic layer (Person 2)
    loginWindow->showLoading(true);
    
    // Example: After successful login from logic layer:
    // loginWindow->hide();
    // setCentralWidget(chatWindow);
}

void MainWindow::handleLogout()
{
    chatWindow->hide();
    setCentralWidget(loginWindow);
    loginWindow->clearFields();
}
