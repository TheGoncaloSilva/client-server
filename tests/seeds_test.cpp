#include <gtest/gtest.h>

#include "seeds.h"
//#include "../include/seeds.h"

TEST(Seeds, TestClassCreation)
{
    Seeds s{CLIENT};
    EXPECT_EQ(s.handler, CLIENT);

    s = Seeds(SERVER);
    EXPECT_EQ(s.handler, SERVER);
}

TEST(Seeds, TestRefreshSeedsSuccess)
{
    Seeds s{SERVER};
    s.refresh_seeds();
    EXPECT_NEAR(s.get_seed_pe(), 0, 1024);
    EXPECT_NEAR(s.get_seed_mux(), 0, 26);
}

TEST(Seeds, TestRefreshSeedsFail)
{
    // Check if assert was triggered
    EXPECT_DEATH({
        Seeds s{CLIENT};
        s.refresh_seeds();
    }, "Only the Server should reset seeds");
}

TEST(Seeds, TestSetSeedsSuccessServer)
{
    Seeds s{SERVER};
    s.set_seed_mux(15);
    EXPECT_EQ(s.get_seed_mux(), 15);
    s.set_seed_pe(15);
    EXPECT_EQ(s.get_seed_pe(), 15);
}

TEST(Seeds, TestSetSeedsSuccessClient)
{
    Seeds s{CLIENT};
    s.set_seed_mux(15);
    EXPECT_EQ(s.get_seed_mux(), 15);
    s.set_seed_pe(15);
    EXPECT_EQ(s.get_seed_pe(), 15);
}

TEST(Seeds, TestSetSeedsFailServer)
{
    EXPECT_DEATH({
        Seeds s{SERVER};
        s.set_seed_mux(10000);
    }, "Value should be within bit size");

    EXPECT_DEATH({
        Seeds s{SERVER};
        s.set_seed_mux(10000);
    }, "Value should be within bit size");
}

TEST(Seeds, TestSetSeedsFailClient)
{
    EXPECT_DEATH({
        Seeds s{CLIENT};
        s.set_seed_mux(10000);
    }, "Value should be within bit size");

    EXPECT_DEATH({
        Seeds s{CLIENT};
        s.set_seed_mux(10000);
    }, "Value should be within bit size");
}
