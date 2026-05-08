cmake_minimum_required(VERSION 3.16)
project(ChatServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_executable(chat_server
    server.cpp
    ChatService.cpp
    JsonProtocol.cpp
)

target_include_directories(chat_server PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
