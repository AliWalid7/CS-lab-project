#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LoginWindow.h"
#include "ChatWindow.h"
#include "SettingsWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginRequest(const QString &username, const QString &password);
    void handleLogout();

private:
    LoginWindow *loginWindow;
    ChatWindow *chatWindow;
    SettingsWindow *settingsWindow;
};

#endif
