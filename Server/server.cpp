#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;

awaitable<void> handle_client(tcp::socket socket) {
    char data[1024];

    auto [ec, bytes_read] = co_await socket.async_read_some(
        boost::asio::buffer(data)
    );

    if (!ec) {
        std::string message(data, bytes_read);
        std::cout << "Received: " << message << std::endl;
        
        std::string response = "OK: " + message;
        co_await socket.async_write_some(
            boost::asio::buffer(response)
        );
    }
}

awaitable<void> listener() {
    auto io_ctx = co_await boost::asio::this_coro::executor;

    tcp::acceptor acceptor(io_ctx, { tcp::v4(), 54321 });

    std::cout << "Server is listening on 127.0.0.1: port 54321...\n";

    while (true) {
        // Accept a new connection
        auto [ec, socket] = co_await acceptor.async_accept(as_tuple(use_awaitable));

        if (!ec) {
            // Spawn a new coroutine to handle this specific client
            co_spawn(io_ctx, handle_client(std::move(socket)), detached);
        }
        else {
            std::cout << "Accept error: " << ec.message() << "\n";
        }
    }
}



int main() {
    boost::asio::io_context io_context;

    // Start the main listener coroutine
    co_spawn(io_context, listener(), detached);

    // Run the event loop
    io_context.run();

    return 0;
}
}
