cmake_minimum_required(VERSION 3.20)
project(QtChatClient VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    Network
)

# QCoro
find_package(QCoro6 REQUIRED COMPONENTS Core Network)

# Add subdirectories (must match folder names exactly)
add_subdirectory(src/core)
add_subdirectory(src/network)
add_subdirectory(src/gui)

# Main executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Link everything
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    QCoro::Core
    QCoro::Network

    core        
    network     
    gui         
)
