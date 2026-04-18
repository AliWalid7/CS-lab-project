#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginRequested(const QString &username, const QString &password);
    void switchToRegister();

public slots:
    void showError(const QString &message);
    void showLoading(bool loading);
    void clearFields();

private slots:
    void onLoginButtonClicked();

private:
    Ui::LoginWindow *ui;
    AuthManager *authManager;
};

#endif

