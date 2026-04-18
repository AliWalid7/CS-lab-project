#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "../core/AuthManager.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWindow)
{
    authManager = new AuthManager(this);
    
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    
    connect(authManager, &AuthManager::validationError, this, &LoginWindow::showError);
    connect(authManager, &AuthManager::loginFailed, this, &LoginWindow::showError);
    
    ui->lblStatus->setStyleSheet("color: red;");
    ui->btnLogin->setEnabled(true);
}

LoginWindow::~LoginWindow() { 
    delete ui; 
    delete authManager;
}

void LoginWindow::onLoginButtonClicked()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text();

    if (username.isEmpty()) {
        showError("Username cannot be empty");
        return;
    }

    authManager->attemptLogin(username, password);
}

void LoginWindow::showError(const QString &message)
{
    ui->lblStatus->setText(message);
    ui->lblStatus->setStyleSheet("color: red;");
}

void LoginWindow::showLoading(bool loading)
{
    ui->btnLogin->setEnabled(!loading);
    ui->lblStatus->setText(loading ? "Connecting..." : "");
}

void LoginWindow::clearFields()
{
    ui->txtUsername->clear();
    ui->txtPassword->clear();
    ui->lblStatus->clear();
}
