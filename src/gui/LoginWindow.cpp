#include "LoginWindow.h"
#include "ui_LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    
    ui->lblStatus->setStyleSheet("color: red;");
    ui->btnLogin->setEnabled(true);
}

LoginWindow::~LoginWindow() { delete ui; }

void LoginWindow::onLoginButtonClicked()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text();

    if (username.isEmpty()) {
        showError("Username cannot be empty");
        return;
    }

    emit loginRequested(username, password);
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
