add_library(network
    JsonNetworkClient.cpp
)

target_include_directories(network PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/network
)

target_link_libraries(network PUBLIC
    Qt6::Core
    Qt6::Network
)