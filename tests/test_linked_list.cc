//
// Created by Nicholas Ives on 06/04/2020.
//

#include <gmock/gmock.h>
#include <LinkedList.h>

 class LinkedListTest : public ::testing::Test {
 protected:
  LinkedList<uint8_t> test_list;
};

TEST_F(LinkedListTest, TestInsertAndDelete) {
  uint8_t i = 0;

  // insert 0 to 9
  for (i=0; i<=9; i++) {
    test_list.insert(i);
  }

  for (i=0; i<=9; i++) {
    test_list.remove(i);
  }

  auto start = test_list.begin();
  auto end = test_list.end();

  // if there was anything in the list, this test fails.
  ASSERT_EQ(start, end);
}

TEST_F(LinkedListTest, TestIterator) {
  uint8_t i = 0;

  // insert 0 to 9
  for (i=0; i<=9; i++) {
    test_list.insert(i);
  }

  i = 9;
  for (auto  n : test_list) {
    ASSERT_EQ(i, *n);
    --i;
  }
}