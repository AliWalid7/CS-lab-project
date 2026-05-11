#include "ChatService.h"
#include "JsonProtocol.h"

#include <boost/asio.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/json.hpp>

#include <deque>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

using boost::asio::awaitable;
using boost::asio::buffer;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::as_tuple;
using boost::asio::ip::tcp;

namespace json = boost::json;

struct ClientConnection {
    int id;
    tcp::socket socket;
    std::string incomingData;
    std::deque<std::string> outgoingMessages;
    bool writeLoopRunning = false;

    ClientConnection(int clientId, tcp::socket clientSocket)
        : id(clientId), socket(std::move(clientSocket)) {}
};

std::unordered_map<int, std::shared_ptr<ClientConnection>> clients;
std::unique_ptr<ChatService> chatService;
int nextClientId = 1;

awaitable<void> write_messages(std::shared_ptr<ClientConnection> client);

void disconnect_client(int clientId) {
    auto it = clients.find(clientId);

    if (it == clients.end()) {
        return;
    }

    std::cout << "Client " << clientId << " disconnected\n";

    boost::system::error_code ignored;
    it->second->socket.close(ignored);

    clients.erase(it);

    if (chatService) {
        chatService->removeClient(clientId);
    }
}

void send_to_client(int clientId, const json::object& message) {
    auto it = clients.find(clientId);

    if (it == clients.end()) {
        return;
    }

    std::shared_ptr<ClientConnection> client = it->second;

    client->outgoingMessages.push_back(JsonProtocol::serializeMessage(message));

    if (!client->writeLoopRunning) {
        client->writeLoopRunning = true;
        co_spawn(client->socket.get_executor(), write_messages(client), detached);
    }
}

awaitable<void> write_messages(std::shared_ptr<ClientConnection> client) {
    while (!client->outgoingMessages.empty()) {
        std::string message = client->outgoingMessages.front();

        auto [ec, bytes_written] = co_await boost::asio::async_write(
            client->socket,
            buffer(message),
            as_tuple(use_awaitable)
        );

        if (ec) {
            std::cout << "Write error for client " << client->id << ": "
                      << ec.message() << "\n";

            disconnect_client(client->id);
            co_return;
        }

        client->outgoingMessages.pop_front();
    }

    client->writeLoopRunning = false;
}

void handle_complete_json_line(int clientId, const std::string& line) {
    if (line.empty()) {
        return;
    }

    std::string errorMessage;
    std::optional<json::object> parsedMessage =
        JsonProtocol::parseLine(line, errorMessage);

    if (!parsedMessage.has_value()) {
        send_to_client(clientId, {
            {"type", "error"},
            {"message", errorMessage}
        });
        return;
    }

    chatService->handleMessage(clientId, *parsedMessage);
}

awaitable<void> handle_client(std::shared_ptr<ClientConnection> client) {
    std::cout << "Client " << client->id << " connected\n";

    char data[1024];

    while (true) {
        auto [ec, bytes_read] = co_await client->socket.async_read_some(
            buffer(data),
            as_tuple(use_awaitable)
        );

        if (ec) {
            std::cout << "Read error for client " << client->id << ": "
                      << ec.message() << "\n";

            disconnect_client(client->id);
            co_return;
        }

        client->incomingData.append(data, bytes_read);

        std::size_t newlinePosition;

        while ((newlinePosition = client->incomingData.find('\n')) != std::string::npos) {
            std::string line = client->incomingData.substr(0, newlinePosition);
            client->incomingData.erase(0, newlinePosition + 1);

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            handle_complete_json_line(client->id, line);
        }
    }
}

awaitable<void> listener(unsigned short port) {
    auto ioContext = co_await boost::asio::this_coro::executor;

    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), port));

    std::cout << "Server is listening on port " << port << "...\n";

    while (true) {
        auto [ec, socket] = co_await acceptor.async_accept(
            as_tuple(use_awaitable)
        );

        if (ec) {
            std::cout << "Accept error: " << ec.message() << "\n";
            continue;
        }

        int clientId = nextClientId++;

        auto client = std::make_shared<ClientConnection>(
            clientId,
            std::move(socket)
        );

        clients[clientId] = client;
        chatService->addClient(clientId);

        co_spawn(ioContext, handle_client(client), detached);
    }
}

int main(int argc, char* argv[]) {
    unsigned short port = 54321;

    if (argc > 1) {
        port = static_cast<unsigned short>(std::stoi(argv[1]));
    }

    chatService = std::make_unique<ChatService>(send_to_client);

    boost::asio::io_context ioContext;

    co_spawn(ioContext, listener(port), detached);

    ioContext.run();

    return 0;
}
