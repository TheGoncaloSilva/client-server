#include <gtest/gtest.h>

//#include "seeds.h"
#include "../include/seeds.h"

TEST(Seeds, TestClassCreation)
{
    auto s = Seeds(CLIENT);
    EXPECT_EQ(s.handler, CLIENT);
}
