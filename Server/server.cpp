#include "ChatService.h"
#include "JsonProtocol.h"

#include <boost/asio.hpp>
#include <boost/json/src.hpp>

#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

using boost::asio::ip::tcp;
namespace json = boost::json;

class TcpServer;

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    TcpSession(int id, tcp::socket socket, TcpServer& server)
        : id_(id), socket_(std::move(socket)), server_(server) {}

    int id() const { return id_; }
    void start();
    void deliver(const json::object& message);

private:
    void readLine();
    void writeNext();
    void closeFromError(const boost::system::error_code& ec);

    int id_;
    tcp::socket socket_;
    TcpServer& server_;
    boost::asio::streambuf buffer_;
    std::deque<std::string> outbox_;
};

class TcpServer {
public:
    TcpServer(boost::asio::io_context& io, unsigned short port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)),
          chatService_([this](int clientId, const json::object& message) {
              sendToClient(clientId, message);
          }) {
        std::cout << "Server listening on port " << port << "\n";
        acceptClients();
    }

    void onClientConnected(const std::shared_ptr<TcpSession>& session) {
        sessions_[session->id()] = session;
        chatService_.addClient(session->id());
    }

    void onClientDisconnected(int clientId) {
        sessions_.erase(clientId);
        chatService_.removeClient(clientId);
        std::cout << "Client " << clientId << " disconnected\n";
    }

    void onMessageReceived(int clientId, const json::object& message) {
        chatService_.handleMessage(clientId, message);
    }

private:
    void acceptClients() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                int clientId = nextClientId_++;
                auto session = std::make_shared<TcpSession>(clientId, std::move(socket), *this);
                onClientConnected(session);
                session->start();
            } else {
                std::cerr << "Accept error: " << ec.message() << "\n";
            }

            acceptClients();
        });
    }

    void sendToClient(int clientId, const json::object& message) {
        auto it = sessions_.find(clientId);
        if (it != sessions_.end()) {
            it->second->deliver(message);
        }
    }

    tcp::acceptor acceptor_;
    int nextClientId_ = 1;
    std::unordered_map<int, std::shared_ptr<TcpSession>> sessions_;
    ChatService chatService_;
};

void TcpSession::start() {
    std::cout << "Client " << id_ << " connected: " << socket_.remote_endpoint() << "\n";
    readLine();
}

void TcpSession::deliver(const json::object& message) {
    std::string data = JsonProtocol::serializeMessage(message);
    bool writeInProgress = !outbox_.empty();
    outbox_.push_back(std::move(data));

    if (!writeInProgress) {
        writeNext();
    }
}

void TcpSession::readLine() {
    auto self = shared_from_this();

    boost::asio::async_read_until(socket_, buffer_, '\n',
        [this, self](boost::system::error_code ec, std::size_t) {
            if (ec) {
                closeFromError(ec);
                return;
            }

            std::istream input(&buffer_);
            std::string line;
            std::getline(input, line);

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            std::string errorMessage;
            std::optional<json::object> message = JsonProtocol::parseLine(line, errorMessage);

            if (!message.has_value()) {
                deliver({{"type", "error"}, {"message", errorMessage}});
            } else {
                server_.onMessageReceived(id_, *message);
            }

            readLine();
        });
}

void TcpSession::writeNext() {
    auto self = shared_from_this();

    boost::asio::async_write(socket_, boost::asio::buffer(outbox_.front()),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (ec) {
                closeFromError(ec);
                return;
            }

            outbox_.pop_front();
            if (!outbox_.empty()) {
                writeNext();
            }
        });
}

void TcpSession::closeFromError(const boost::system::error_code& ec) {
    if (ec != boost::asio::error::operation_aborted) {
        server_.onClientDisconnected(id_);
    }
}

int main(int argc, char* argv[]) {
    try {
        unsigned short port = 54321;
        if (argc > 1) {
            port = static_cast<unsigned short>(std::stoi(argv[1]));
        }

        boost::asio::io_context io;
        TcpServer server(io, port);
        io.run();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
