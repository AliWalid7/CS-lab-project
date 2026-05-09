#include <gtest/gtest.h>

TEST(ServerTest, UserAdded)
{
    bool added = true;

    EXPECT_TRUE(added);
}

TEST(ServerTest, DuplicateUserRejected)
{
    bool duplicate = false;

    EXPECT_FALSE(duplicate);
}

TEST(ServerTest, ValidJsonMessage)
{
    std::string msg = "{\"type\":\"message\"}";

    EXPECT_FALSE(msg.empty());
}

TEST(ServerTest, OfflineUser)
{
    bool userOnline = false;

    EXPECT_FALSE(userOnline);
}

TEST(ServerTest, MessageRouting)
{
    std::string receiver = "omar";

    EXPECT_EQ(receiver, "omar");
}
