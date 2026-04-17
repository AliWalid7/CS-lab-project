add_library(network
    JsonNetworkClient.cpp
)

target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(network
    Qt6::Network
    QCoro::Core
    QCoro::Network
)
