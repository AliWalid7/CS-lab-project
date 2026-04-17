#include "mainwindow.h"
#include "chatcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ChatController controller;
    QObject::connect(&controller, &ChatController::onlineUsersChanged,
                     &w, &MainWindow::updateUserList);
    w.show();
    return a.exec();
}
