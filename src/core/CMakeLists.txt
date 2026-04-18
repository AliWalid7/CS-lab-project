
add_library(core
    AppState.cpp
    AuthManager.cpp
    ChatManager.cpp
    Message.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

