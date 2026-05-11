#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "LoginWindow.h"
#include "ChatWindow.h"
#include "ClientController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUi();
    void switchToScreen(const QString &screenName);

    QStackedWidget *stackedWidget;
    LoginWindow *loginWindow;
    ChatWindow *chatWindow;
    ClientController *controller;

};

#endif
