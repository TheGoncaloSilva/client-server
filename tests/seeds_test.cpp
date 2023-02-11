#include <gtest/gtest.h>

#include "seeds.h"
//#include "../include/seeds.h"

TEST(Seeds, TestClassCreation)
{
    Seeds s{CLIENT};
    EXPECT_EQ(s.handler, CLIENT);
}
