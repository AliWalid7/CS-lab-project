#include "ChatWindow.h"
#include "ui_ChatWindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    connect(ui->btnSend, &QPushButton::clicked, this, &ChatWindow::onSendButtonClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &ChatWindow::logoutRequested);
    connect(ui->btnSettings, &QPushButton::clicked, this, &ChatWindow::settingsRequested);
}

void ChatWindow::addMessage(const QString &sender, const QString &message, bool isMine)
{
    QString formatted = isMine 
        ? QString("<p><b>You:</b> %1</p>").arg(message)
        : QString("<p><b>%1:</b> %2</p>").arg(sender, message);
    
    ui->txtChat->append(formatted);
}

void ChatWindow::updateUserList(const QStringList &users)
{
    ui->listUsers->clear();
    ui->listUsers->addItems(users);
}

void ChatWindow::onSendButtonClicked()
{
    QString text = ui->txtMessageInput->text().trimmed();
    if (!text.isEmpty()) {
        emit sendMessageRequested(text);
        ui->txtMessageInput->clear();
    }
}
