#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include "../core/ChatManager.h"
#include "../network/JsonNetworkClient.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatWindow), chatManager(nullptr), networkClient(nullptr)
{
    ui->setupUi(this);
    
    chatManager = new ChatManager(this);
    networkClient = new JsonNetworkClient(this);
    
    connect(ui->btnSend, &QPushButton::clicked, this, &ChatWindow::onSendButtonClicked);
    connect(ui->btnLogout, &QPushButton::clicked, this, &ChatWindow::logoutRequested);
    connect(ui->btnSettings, &QPushButton::clicked, this, &ChatWindow::settingsRequested);
    
    connect(chatManager, &ChatManager::historyChanged, this, &ChatWindow::updateChatHistory);
    connect(chatManager, &ChatManager::usersChanged, this, &ChatWindow::updateUserList);
    
    connect(networkClient, &JsonNetworkClient::messageReceived, this, &ChatWindow::addMessage);
    connect(networkClient, &JsonNetworkClient::connected, this, &ChatWindow::showConnectionStatus);
    connect(networkClient, &JsonNetworkClient::disconnected, this, &ChatWindow::showConnectionStatus);
    connect(networkClient, &JsonNetworkClient::errorOccurred, this, &ChatWindow::showConnectionStatus);
}

ChatWindow::~ChatWindow() { 
    delete ui; 
    delete chatManager;
    delete networkClient;
}

void ChatWindow::onSendButtonClicked()
{
    QString text = ui->txtMessageInput->text();
    if (!text.isEmpty()) {
        chatManager->addMessage("CurrentUser", text);
        networkClient->sendChatMessage("CurrentUser", text);
        ui->txtMessageInput->clear();
    }
}

void ChatWindow::addMessage(const QString &sender, const QString &message, bool isMine)
{
    QString formatted = isMine 
        ? "You: " + message
        : sender + ": " + message;
    
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
    ui->lblStatus->setText(status);
    ui->btnSend->setEnabled(isConnected);
}
