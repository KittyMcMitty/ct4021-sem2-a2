//
// Created by Nicholas Ives on 25/03/2020.
//

#include "gmock/gmock.h"
#include <SomeClass.h>

 class SomeClassTest : public ::testing::Test {
  protected:

};

TEST_F(SomeClassTest, TestIncrement) {
  SomeClass my_class(1);
  uint8_t num = my_class.increment();

  EXPECT_EQ(num, 2);
}