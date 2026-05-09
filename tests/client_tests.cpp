#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockNetworkClient.h"

using namespace testing;

TEST(ClientTest, ReceiveMessage)
{
    MockNetworkClient mock;

    EXPECT_CALL(mock, receive())
        .WillOnce(Return("{\"message\":\"hello\"}"));

    std::string result = mock.receive();

    EXPECT_EQ(result, "{\"message\":\"hello\"}");
}

TEST(ClientTest, LoginSuccess)
{
    MockNetworkClient mock;

    EXPECT_CALL(mock, receive())
        .WillOnce(Return("OK"));

    EXPECT_EQ(mock.receive(), "OK");
}

TEST(ClientTest, LoginFailure)
{
    MockNetworkClient mock;

    EXPECT_CALL(mock, receive())
        .WillOnce(Return("ERROR"));

    EXPECT_EQ(mock.receive(), "ERROR");
}

TEST(ClientTest, SendMessage)
{
    MockNetworkClient mock;

    EXPECT_CALL(mock, send("{\"msg\":\"hi\"}"));

    mock.send("{\"msg\":\"hi\"}");
}

TEST(ClientTest, InvalidJson)
{
    MockNetworkClient mock;

    EXPECT_CALL(mock, receive())
        .WillOnce(Return("INVALID_JSON"));

    EXPECT_EQ(mock.receive(), "INVALID_JSON");
}
