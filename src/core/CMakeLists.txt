add_library(core
    AppState.cpp
    AuthManager.cpp
    ChatManager.cpp
    Message.cpp

)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core
)

target_link_libraries(core PUBLIC
    Qt6::Core
    network
)