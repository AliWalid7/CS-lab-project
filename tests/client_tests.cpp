#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QString>

#include "AuthManager.h"
#include "ChatManager.h"

using namespace testing;

TEST(ClientBusinessLogicTest, UsernameValidation) {
    QString error;
    
    EXPECT_FALSE(AuthManager::validateUsername("ab", &error));
    EXPECT_FALSE(error.isEmpty());
    
    EXPECT_TRUE(AuthManager::validateUsername("validuser", &error));
    EXPECT_TRUE(error.isEmpty());
}

TEST(ClientBusinessLogicTest, PasswordValidation) {
    QString error;
    
    EXPECT_FALSE(AuthManager::validatePassword("123", &error));
    EXPECT_FALSE(error.isEmpty());
    
    EXPECT_TRUE(AuthManager::validatePassword("validpass", &error));
    EXPECT_TRUE(error.isEmpty());
}

TEST(ClientBusinessLogicTest, ChatManagerUserManagement) {
    ChatManager chatManager;

    EXPECT_TRUE(chatManager.addUser("user1"));
    EXPECT_TRUE(chatManager.addUser("user2"));
    EXPECT_FALSE(chatManager.addUser("user1"));
    EXPECT_FALSE(chatManager.addUser(""));

    auto users = chatManager.getUsers();
    EXPECT_EQ(users.size(), 2);
    EXPECT_TRUE(users.contains("user1"));
    EXPECT_TRUE(users.contains("user2"));

    EXPECT_TRUE(chatManager.removeUser("user1"));
    EXPECT_FALSE(chatManager.removeUser("nonexistent"));

    users = chatManager.getUsers();
    EXPECT_EQ(users.size(), 1);
    EXPECT_FALSE(users.contains("user1"));
}

TEST(ClientBusinessLogicTest, ChatManagerMessageManagement) {
    ChatManager chatManager;

    EXPECT_TRUE(chatManager.addMessage("user1", "Hello"));
    EXPECT_FALSE(chatManager.addMessage("", "Hello"));
    EXPECT_FALSE(chatManager.addMessage("user1", ""));

    auto history = chatManager.getHistory();
    EXPECT_EQ(history.size(), 1);
    EXPECT_EQ(history[0].username, "user1");
    EXPECT_EQ(history[0].text, "Hello");

    chatManager.clearHistory();
    history = chatManager.getHistory();
    EXPECT_EQ(history.size(), 0);
}

TEST(ClientBusinessLogicTest, ChatManagerCaseInsensitiveUserCheck) {
    ChatManager chatManager;

    EXPECT_TRUE(chatManager.addUser("User1"));
    EXPECT_FALSE(chatManager.addUser("user1"));
    EXPECT_FALSE(chatManager.addUser("USER1"));

    auto users = chatManager.getUsers();
    EXPECT_EQ(users.size(), 1);
}
