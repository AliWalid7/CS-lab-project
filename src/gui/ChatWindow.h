#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

signals:
    void sendMessageRequested(const QString &message);
    void logoutRequested();
    void settingsRequested();

public slots:
    void addMessage(const QString &sender, const QString &message, bool isMine);
    void updateUserList(const QStringList &users);
    void showConnectionStatus(const QString &status, bool isConnected);

private slots:
    void onSendButtonClicked();

private:
    Ui::ChatWindow *ui;
};

#endif
