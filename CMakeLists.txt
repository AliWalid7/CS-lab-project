cmake_minimum_required(VERSION 3.20)
project(QtChatClient VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Test
)

# QCoro disabled for testing
# find_package(QCoro6 REQUIRED COMPONENTS Core Network)
find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS json)

# Modules
add_subdirectory(src/core)
add_subdirectory(src/network)
add_subdirectory(src/gui)
add_subdirectory(Server)

# =========================
# MAIN APPLICATION ENABLED
# =========================

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    gui
    core
    network

    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

# =========================
# TESTING
# Unit testing configuration
# =========================

enable_testing()

# Try to find GTest first
find_package(GTest QUIET)

# If not found, download and build it using FetchContent
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

include_directories(
    include
    tests
)

add_executable(runTests
    tests/client_tests.cpp
    tests/server_tests.cpp
)

target_include_directories(runTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/network
    ${CMAKE_SOURCE_DIR}/src/gui
)

target_link_libraries(runTests PRIVATE
    gtest
    gmock
    gtest_main
    core
    network
    Qt6::Core
    Qt6::Network
    Qt6::Test
    Boost::json
)

add_test(NAME runTests COMMAND runTests)
