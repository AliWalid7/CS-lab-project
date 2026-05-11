add_library(network
    JsonNetworkClient.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Message.cpp
    ${CMAKE_SOURCE_DIR}/include/JsonNetworkClient.h
    ${CMAKE_SOURCE_DIR}/include/Message.h
)

target_include_directories(network PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/network
)

target_link_libraries(network PUBLIC
    Qt6::Core
    Qt6::Network
)
