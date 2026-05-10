
#pragma once

#include <gmock/gmock.h>
#include <string>

class MockNetworkClient {
public:
    MOCK_METHOD(void, send, (const std::string& msg));
    MOCK_METHOD(std::string, receive, ());
};
