#include "mainwindow.h"
#include "chatmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //ChatManager controller;
    //&ChatManager::onlineUsersChanged,
    //                 &w, &MainWindow::updateUserList);
    w.show();
    return a.exec();
}
