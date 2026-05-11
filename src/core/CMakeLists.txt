add_library(core
    AppState.cpp
    AuthManager.cpp
    ChatManager.cpp
    ClientController.cpp
    DatabaseManager.cpp
    ${CMAKE_SOURCE_DIR}/include/AppState.h
    ${CMAKE_SOURCE_DIR}/include/AuthManager.h
    ${CMAKE_SOURCE_DIR}/include/ChatManager.h
    ${CMAKE_SOURCE_DIR}/include/ClientController.h
    ${CMAKE_SOURCE_DIR}/include/Message.h
)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core
)

set_target_properties(core PROPERTIES AUTOMOC ON)
set_target_properties(core PROPERTIES AUTORCC ON)
set_target_properties(core PROPERTIES AUTOUIC ON)

target_link_libraries(core PUBLIC
    Qt6::Core
    network
    SQLite::SQLite3
)
