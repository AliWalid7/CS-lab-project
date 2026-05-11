
add_library(gui
    LoginWindow.cpp
    ChatWindow.cpp
    MainWindow.cpp
    ../../include/LoginWindow.h
    ../../include/ChatWindow.h
    ../../include/MainWindow.h
    ../../ui/LoginWindow.ui
    ../../ui/ChatWindow.ui
)

set(CMAKE_AUTOUIC ON)
set_target_properties(gui PROPERTIES AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../../ui")
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ui)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../src/core)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../src/network)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui PUBLIC
    core
    network
    Qt6::Widgets
    Qt6::Network
)
