#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QString>
#include <QStringList>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();
    
    //ugghh communication w/ mainwind manager
    Q_SIGNAL void sendMessageRequested(const QString &message);
    Q_SIGNAL void logoutRequested();

public slots:
    void addMessage(const QString &sender, const QString &message, bool isMine);
    void updateUserList(const QStringList &users);
    void showConnectionStatus(const QString &status, bool isConnected);
    void updateChatHistory(const QList<Message> &history);
    
private slots:
    void onSendButtonClicked();

private:
    Ui::ChatWindow *ui;
    
};

#endif
