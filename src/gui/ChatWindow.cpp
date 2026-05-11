#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include "ChatManager.h"
#include "JsonNetworkClient.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    
    connect(ui->btnSend, &QPushButton::clicked, this, &ChatWindow::onSendButtonClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &ChatWindow::logoutRequested);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::onSendButtonClicked()
{
    QString text = ui->txtMessageInput->text().trimmed();
    if (!text.isEmpty()) {
        emit sendMessageRequested(text);
        ui->txtMessageInput->clear();
    }
}

void ChatWindow::addMessage(const QString &sender, const QString &message, bool isMine)
{
    QString formatted;
    if (isMine) {
        formatted = QString("<p><b style='color:blue;'>You:</b> %1</p>").arg(message.toHtmlEscaped());
    } else {
        formatted = QString("<p><b>%1:</b> %2</p>").arg(sender.toHtmlEscaped(), message.toHtmlEscaped());
    }
    ui->txtChat->append(formatted);
}

void ChatWindow::updateUserList(const QStringList &users)
{
    ui->listUsers->clear();
    ui->listUsers->addItems(users);
}

void ChatWindow::updateChatHistory(const QList<Message> &history)
{
    ui->txtChat->clear();
    for (const auto &msg : history) {
        addMessage(msg.username, msg.text, false);
    }
}

void ChatWindow::showConnectionStatus(const QString &status, bool isConnected)
{
    QString color = isConnected ? "#2E7D32" : "#C62828";
    ui->lblUsers->setText(QString("<span style='color:%1;'>%2</span>").arg(color, status.toHtmlEscaped()));
}

void ChatWindow::clearChat()
{
    ui->txtChat->clear();
    ui->listUsers->clear();
    ui->lblUsers->setText("Online Users");
}
