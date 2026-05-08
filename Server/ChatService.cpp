#include "ChatService.h"

#include <algorithm>

ChatService::ChatService(SendCallback sendCallback)
    : sendCallback_(std::move(sendCallback)) {}

void ChatService::addClient(int clientId) {
    clientToUsername_.try_emplace(clientId, "");
}

void ChatService::removeClient(int clientId) {
    auto clientIt = clientToUsername_.find(clientId);
    if (clientIt == clientToUsername_.end()) {
        return;
    }

    std::string username = clientIt->second;
    clientToUsername_.erase(clientIt);

    if (!username.empty()) {
        usernameToClient_.erase(username);
        broadcast({{"type", "user_left"}, {"username", username}});
        broadcast({{"type", "broadcast"}, {"username", "Server"}, {"text", username + " left the chat"}});
        broadcastUserList();
    }
}

void ChatService::handleMessage(int clientId, const json::object& message) {
    if (!hasStringField(message, "type")) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Message must contain a string type"}});
        return;
    }

    const std::string type = getStringField(message, "type");

    if (type == "join") {
        handleJoin(clientId, message);
    } else if (type == "message") {
        handleChatMessage(clientId, message);
    } else {
        sendToClient(clientId, {{"type", "error"}, {"message", "Unknown message type"}});
    }
}

void ChatService::handleJoin(int clientId, const json::object& message) {
    if (!hasStringField(message, "username")) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Join request needs a username"}});
        return;
    }

    std::string username = getStringField(message, "username");
    if (username.empty()) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Username cannot be empty"}});
        return;
    }

    if (usernameToClient_.contains(username)) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Username already taken"}});
        return;
    }

    auto clientIt = clientToUsername_.find(clientId);
    if (clientIt == clientToUsername_.end()) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Client is not registered"}});
        return;
    }

    if (!clientIt->second.empty()) {
        usernameToClient_.erase(clientIt->second);
    }

    clientIt->second = username;
    usernameToClient_[username] = clientId;

    sendToClient(clientId, {{"type", "join_success"}});
    broadcast({{"type", "broadcast"}, {"username", "Server"}, {"text", username + " joined the chat"}});
    broadcastUserList();
}

void ChatService::handleChatMessage(int clientId, const json::object& message) {
    auto clientIt = clientToUsername_.find(clientId);
    if (clientIt == clientToUsername_.end() || clientIt->second.empty()) {
        sendToClient(clientId, {{"type", "error"}, {"message", "You must join before sending messages"}});
        return;
    }

    if (!hasStringField(message, "text")) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Message needs text"}});
        return;
    }

    const std::string text = getStringField(message, "text");
    if (text.empty()) {
        sendToClient(clientId, {{"type", "error"}, {"message", "Message cannot be empty"}});
        return;
    }

    json::object outgoing{{"type", "broadcast"}, {"username", clientIt->second}, {"text", text}};

    if (hasStringField(message, "to")) {
        const std::string recipient = getStringField(message, "to");
        if (!usernameToClient_.contains(recipient)) {
            sendToClient(clientId, {{"type", "error"}, {"message", "Recipient not found"}});
            return;
        }

        sendToUser(recipient, outgoing);
        if (recipient != clientIt->second) {
            sendToClient(clientId, outgoing);
        }
    } else {
        broadcast(outgoing);
    }
}

void ChatService::sendToClient(int clientId, const json::object& message) {
    if (sendCallback_) {
        sendCallback_(clientId, message);
    }
}

void ChatService::sendToUser(const std::string& username, const json::object& message) {
    auto it = usernameToClient_.find(username);
    if (it != usernameToClient_.end()) {
        sendToClient(it->second, message);
    }
}

void ChatService::broadcast(const json::object& message) {
    for (const auto& [clientId, username] : clientToUsername_) {
        sendToClient(clientId, message);
    }
}

void ChatService::broadcastUserList() {
    json::array users;
    std::vector<std::string> sortedUsers;

    for (const auto& [username, clientId] : usernameToClient_) {
        sortedUsers.push_back(username);
    }

    std::sort(sortedUsers.begin(), sortedUsers.end());

    for (const std::string& username : sortedUsers) {
        users.push_back(json::value(username));
    }

    broadcast({{"type", "user_list"}, {"users", users}});
}

bool ChatService::hasStringField(const json::object& message, const char* fieldName) {
    const json::value* value = message.if_contains(fieldName);
    return value != nullptr && value->is_string();
}

std::string ChatService::getStringField(const json::object& message, const char* fieldName) {
    const json::value* value = message.if_contains(fieldName);
    if (!value || !value->is_string()) {
        return "";
    }

    return json::value_to<std::string>(*value);
}
