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

# QCoro disabled for testing
# find_package(QCoro6 REQUIRED COMPONENTS Core Network)

# Modules
add_subdirectory(src/core)
add_subdirectory(src/network)

# GUI disabled temporarily
# add_subdirectory(src/gui)

# =========================
# MAIN APPLICATION DISABLED
# =========================

# add_executable(${PROJECT_NAME}
#     src/main.cpp
# )

# target_link_libraries(${PROJECT_NAME} PRIVATE
#     Qt6::Core
#     Qt6::Gui
#     Qt6::Widgets
#     Qt6::Network
#
#     core
#     network
# )

# =========================
# TESTING
# =========================

enable_testing()

find_package(GTest REQUIRED)

include_directories(
    include
    tests
)

add_executable(runTests
    tests/client_tests.cpp
    tests/server_tests.cpp
)

target_link_libraries(runTests PRIVATE
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
    core
    network
    Qt6::Core
    Qt6::Network
)

add_test(NAME runTests COMMAND runTests)