#ifndef CHATROOMSCREEN_H
#define CHATROOMSCREEN_H

#include <QWidget>
#include <QStringList>

class QLabel;
class QListWidget;

class ChatRoomScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ChatRoomScreen(QWidget *parent = nullptr);
    void updateUserList(const QStringList &users);

private:
    QLabel *m_titleLabel;
    QListWidget *m_userListWidget;
};

#endif
