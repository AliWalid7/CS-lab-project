#include "ChatRoomScreen.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

ChatRoomScreen::ChatRoomScreen(QWidget *parent)
    : QWidget(parent)
{
    m_titleLabel = new QLabel("Online Users", this);
    QFont font = m_titleLabel->font();
    font.setPointSize(14);
    font.setBold(true);
    m_titleLabel->setFont(font);

    m_userListWidget = new QListWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_userListWidget);
}

void ChatRoomScreen::updateUserList(const QStringList &users)
{
    m_userListWidget->clear();
    for (int i = 0; i < users.size(); ++i)
        m_userListWidget->addItem(users.at(i));
}
