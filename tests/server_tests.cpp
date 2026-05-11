#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/json.hpp>
#include <unordered_map>
#include <functional>

using namespace testing;
namespace json = boost::json;

class MockSendCallback {
public:
    MOCK_METHOD(void, sendMessage, (int clientId, const json::object& message));
};

class TestableChatService {
private:
    std::unordered_map<int, std::string> clientToUsername_;
    std::unordered_map<std::string, int> usernameToClient_;
    MockSendCallback* callback_;
    
public:
    explicit TestableChatService(MockSendCallback* callback) : callback_(callback) {}

    bool addUser(int clientId, const std::string& username) {
        if (usernameToClient_.find(username) != usernameToClient_.end()) {
            return false;
        }
        clientToUsername_[clientId] = username;
        usernameToClient_[username] = clientId;
        return true;
    }

    void removeUser(int clientId) {
        auto it = clientToUsername_.find(clientId);
        if (it != clientToUsername_.end()) {
            usernameToClient_.erase(it->second);
            clientToUsername_.erase(it);
        }
    }

    bool routeMessage(int senderId, const std::string& receiver, const json::object& message) {
        if (clientToUsername_.find(senderId) == clientToUsername_.end()) {
            return false;
        }

        auto receiverIt = usernameToClient_.find(receiver);
        if (receiverIt == usernameToClient_.end()) {
            return false;
        }

        if (callback_) {
            callback_->sendMessage(receiverIt->second, message);
        }
        return true;
    }

    bool isUserOnline(const std::string& username) const {
        return usernameToClient_.find(username) != usernameToClient_.end();
    }

    size_t getUserCount() const {
        return clientToUsername_.size();
    }
};

TEST(ServerBusinessLogicTest, UserAdditionAndRemoval) {
    MockSendCallback mockCallback;
    TestableChatService chatService(&mockCallback);

    EXPECT_TRUE(chatService.addUser(1, "user1"));
    EXPECT_TRUE(chatService.addUser(2, "user2"));

    EXPECT_EQ(chatService.getUserCount(), 2);
    EXPECT_TRUE(chatService.isUserOnline("user1"));
    EXPECT_TRUE(chatService.isUserOnline("user2"));

    chatService.removeUser(1);
    EXPECT_EQ(chatService.getUserCount(), 1);
    EXPECT_FALSE(chatService.isUserOnline("user1"));
    EXPECT_TRUE(chatService.isUserOnline("user2"));
}

TEST(ServerBusinessLogicTest, DuplicateUsernameRejection) {
    MockSendCallback mockCallback;
    TestableChatService chatService(&mockCallback);

    EXPECT_TRUE(chatService.addUser(1, "duplicate_user"));

    EXPECT_FALSE(chatService.addUser(2, "duplicate_user"));

    EXPECT_EQ(chatService.getUserCount(), 1);
    EXPECT_TRUE(chatService.isUserOnline("duplicate_user"));
}

TEST(ServerBusinessLogicTest, MessageRoutingBetweenUsers) {
    MockSendCallback mockCallback;
    TestableChatService chatService(&mockCallback);

    chatService.addUser(1, "sender");
    chatService.addUser(2, "receiver");

    json::object message;
    message["type"] = "chat";
    message["text"] = "Hello!";

    EXPECT_CALL(mockCallback, sendMessage(2, _))
        .Times(1);

    bool routed = chatService.routeMessage(1, "receiver", message);
    EXPECT_TRUE(routed);
}

TEST(ServerBusinessLogicTest, UnauthenticatedMessageRejection) {
    MockSendCallback mockCallback;
    TestableChatService chatService(&mockCallback);

    chatService.addUser(2, "receiver");

    json::object message;
    message["type"] = "chat";
    message["text"] = "Unauthorized";

    EXPECT_CALL(mockCallback, sendMessage(_, _))
        .Times(0);

    bool routed = chatService.routeMessage(1, "receiver", message);
    EXPECT_FALSE(routed);
}

TEST(ServerBusinessLogicTest, MessageToOfflineUser) {
    MockSendCallback mockCallback;
    TestableChatService chatService(&mockCallback);

    chatService.addUser(1, "sender");

    json::object message;
    message["type"] = "chat";
    message["text"] = "Hello offline user";

    EXPECT_CALL(mockCallback, sendMessage(_, _))
        .Times(0);

    bool routed = chatService.routeMessage(1, "offline_user", message);
    EXPECT_FALSE(routed);
}
