add_library(core
    AppState.cpp
    AuthManager.cpp
    chatmanager.cpp
    Message.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(core PRIVATE Qt6::Core)
