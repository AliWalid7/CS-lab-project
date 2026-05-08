cmake_minimum_required(VERSION 3.16)
project(ChatServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(chat_server server.cpp)

target_compile_definitions(chat_server PRIVATE
    BOOST_ERROR_CODE_HEADER_ONLY
    BOOST_SYSTEM_NO_LIB
    BOOST_JSON_HEADER_ONLY
)

target_link_libraries(chat_server PRIVATE Threads::Threads)
