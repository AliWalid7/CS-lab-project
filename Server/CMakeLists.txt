cmake_minimum_required(VERSION 3.20)
project(ChatServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS json)
find_package(Threads REQUIRED)

add_executable(chat_server
    server.cpp
    ChatService.cpp
    JsonProtocol.cpp
)

target_include_directories(chat_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(chat_server PRIVATE
    Boost::json
    Threads::Threads
)

# Windows-only socket libraries
if(WIN32)
    target_link_libraries(chat_server PRIVATE
        ws2_32
        mswsock
    )
endif()