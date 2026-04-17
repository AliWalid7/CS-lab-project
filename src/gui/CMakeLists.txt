add_library(gui
    LoginScreen.cpp
    ChatRoomScreen.cpp
    SettingsScreen.cpp
    MainWindow.cpp
)

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui
    Qt6::Widgets
)
