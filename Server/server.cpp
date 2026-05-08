#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/json/src.hpp>

#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

using boost::asio::ip::tcp;
namespace json = boost::json;

class ChatSession;

class ChatServer {
public:
    ChatServer(boost::asio::io_context& io, unsigned short port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "Server listening on port " << port << "\n";
        acceptClients();
    }

    void join(const std::shared_ptr<ChatSession>& session, const std::string& username);
    void leave(const std::shared_ptr<ChatSession>& session);
    void routeMessage(const std::shared_ptr<ChatSession>& session, const json::object& message);
    void sendUserList();

private:
    void acceptClients();
    void broadcast(const json::object& message);
    void sendToUser(const std::string& username, const json::object& message);

    tcp::acceptor acceptor_;
    std::set<std::shared_ptr<ChatSession>> sessions_;
    std::unordered_map<std::string, std::shared_ptr<ChatSession>> users_;
};

class ChatSession : public std::enable_shared_from_this<ChatSession> {
public:
    ChatSession(tcp::socket socket, ChatServer& server)
        : socket_(std::move(socket)), server_(server) {}

    void start() {
        std::cout << "Client connected: " << socket_.remote_endpoint() << "\n";
        readLine();
    }

    void deliver(const json::object& message) {
        std::string data = json::serialize(message) + "\n";
        bool writeInProgress = !outbox_.empty();
        outbox_.push_back(std::move(data));

        if (!writeInProgress) {
            writeNext();
        }
    }

    std::string username() const { return username_; }
    void setUsername(std::string username) { username_ = std::move(username); }

private:
    void readLine() {
        auto self = shared_from_this();

        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this, self](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    server_.leave(self);
                    return;
                }

                std::istream input(&buffer_);
                std::string line;
                std::getline(input, line);

                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                handleLine(line);
                readLine();
            });
    }

    void handleLine(const std::string& line) {
        boost::system::error_code ec;
        json::value parsed = json::parse(line, ec);

        if (ec || !parsed.is_object()) {
            deliver({{"type", "error"}, {"message", "Invalid JSON message"}});
            return;
        }

        json::object message = parsed.as_object();
        std::string type = json::value_to<std::string>(message.if_contains("type") ? *message.if_contains("type") : json::value(""));

        if (type == "join") {
            const json::value* usernameValue = message.if_contains("username");
            if (!usernameValue || !usernameValue->is_string()) {
                deliver({{"type", "error"}, {"message", "Join request needs a username"}});
                return;
            }

            server_.join(shared_from_this(), json::value_to<std::string>(*usernameValue));
        } else if (type == "message") {
            server_.routeMessage(shared_from_this(), message);
        } else {
            deliver({{"type", "error"}, {"message", "Unknown message type"}});
        }
    }

    void writeNext() {
        auto self = shared_from_this();

        boost::asio::async_write(socket_, boost::asio::buffer(outbox_.front()),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (ec) {
                    server_.leave(self);
                    return;
                }

                outbox_.pop_front();
                if (!outbox_.empty()) {
                    writeNext();
                }
            });
    }

    tcp::socket socket_;
    ChatServer& server_;
    boost::asio::streambuf buffer_;
    std::deque<std::string> outbox_;
    std::string username_;
};

void ChatServer::acceptClients() {
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            auto session = std::make_shared<ChatSession>(std::move(socket), *this);
            sessions_.insert(session);
            session->start();
        } else {
            std::cerr << "Accept error: " << ec.message() << "\n";
        }

        acceptClients();
    });
}

void ChatServer::join(const std::shared_ptr<ChatSession>& session, const std::string& username) {
    if (username.empty()) {
        session->deliver({{"type", "error"}, {"message", "Username cannot be empty"}});
        return;
    }

    if (users_.contains(username)) {
        session->deliver({{"type", "error"}, {"message", "Username already taken"}});
        return;
    }

    session->setUsername(username);
    users_[username] = session;

    session->deliver({{"type", "join_success"}});
    broadcast({{"type", "broadcast"}, {"username", "Server"}, {"text", username + " joined the chat"}});
    sendUserList();

    std::cout << username << " joined\n";
}

void ChatServer::leave(const std::shared_ptr<ChatSession>& session) {
    if (!sessions_.erase(session)) {
        return;
    }

    std::string username = session->username();
    if (!username.empty()) {
        users_.erase(username);
        broadcast({{"type", "user_left"}, {"username", username}});
        broadcast({{"type", "broadcast"}, {"username", "Server"}, {"text", username + " left the chat"}});
        sendUserList();
        std::cout << username << " disconnected\n";
    } else {
        std::cout << "Unnamed client disconnected\n";
    }
}

void ChatServer::routeMessage(const std::shared_ptr<ChatSession>& session, const json::object& message) {
    if (session->username().empty()) {
        session->deliver({{"type", "error"}, {"message", "You must join before sending messages"}});
        return;
    }

    const json::value* textValue = message.if_contains("text");
    if (!textValue || !textValue->is_string()) {
        session->deliver({{"type", "error"}, {"message", "Message needs text"}});
        return;
    }

    std::string text = json::value_to<std::string>(*textValue);
    json::object outgoing{{"type", "broadcast"}, {"username", session->username()}, {"text", text}};

    // Optional private routing: {"type":"message", "to":"username", "text":"hello"}
    const json::value* toValue = message.if_contains("to");
    if (toValue && toValue->is_string()) {
        sendToUser(json::value_to<std::string>(*toValue), outgoing);
        session->deliver(outgoing);
    } else {
        broadcast(outgoing);
    }
}

void ChatServer::sendUserList() {
    json::array users;
    for (const auto& [username, session] : users_) {
        users.push_back(json::value(username));
    }

    broadcast({{"type", "user_list"}, {"users", users}});
}

void ChatServer::broadcast(const json::object& message) {
    for (const auto& session : sessions_) {
        session->deliver(message);
    }
}

void ChatServer::sendToUser(const std::string& username, const json::object& message) {
    auto it = users_.find(username);
    if (it != users_.end()) {
        it->second->deliver(message);
    }
}

int main(int argc, char* argv[]) {
    try {
        unsigned short port = 54321;
        if (argc > 1) {
            port = static_cast<unsigned short>(std::stoi(argv[1]));
        }

        boost::asio::io_context io;
        ChatServer server(io, port);
        io.run();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
