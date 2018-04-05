#include <gtest/gtest.h>


TEST(Dummy, ExpectPass)
  {
  EXPECT_FALSE(false);
  }

TEST(Dummy, ExpectFail)
  {
  EXPECT_FALSE(true);
  }

TEST(Dummy, AssertPass)
  {
  ASSERT_FALSE(false);
  }

TEST(Dummy, AssertFail)
  {
  ASSERT_FALSE(true);
  }
