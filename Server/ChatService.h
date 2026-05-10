#ifndef CHAT_SERVICE_H
#define CHAT_SERVICE_H

#include <boost/json.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace json = boost::json;

class ChatService {
public:
    using SendCallback = std::function<void(int clientId, const json::object& message)>;

    explicit ChatService(SendCallback sendCallback);

    void addClient(int clientId);
    void removeClient(int clientId);
    void handleMessage(int clientId, const json::object& message);

private:
    void handleJoin(int clientId, const json::object& message);
    void handleChatMessage(int clientId, const json::object& message);

    void sendToClient(int clientId, const json::object& message);
    void sendToUser(const std::string& username, const json::object& message);
    void broadcast(const json::object& message);
    void broadcastUserList();

    static bool hasStringField(const json::object& message, const char* fieldName);
    static std::string getStringField(const json::object& message, const char* fieldName);

    SendCallback sendCallback_;
    std::unordered_map<int, std::string> clientToUsername_;
    std::unordered_map<std::string, int> usernameToClient_;
};

#endif
