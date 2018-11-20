#include <gtest/gtest.h>


TEST(Dummy, ExpectPass)
  {
  EXPECT_FALSE(false);
  }

TEST(Dummy, AssertPass)
  {
  ASSERT_FALSE(false);
  }
