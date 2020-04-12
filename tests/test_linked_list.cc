//
// Created by Nicholas Ives on 06/04/2020.
//

#include <gmock/gmock.h>
#include <LinkedList.h>

 class LinkedListTest : public ::testing::Test {
 protected:
  LinkedList<uint8_t> test_list;
};

TEST_F(LinkedListTest, TestInsertAndIterate) {

  uint8_t i = 0;

  // insert 0 to 9
  for (i=0; i<10; i++) {
    test_list.insert(i);
  }

  // this should clear the iterator and put it back at start
  test_list.reset_iterate();

  // we should now have a list of 9 to 0 (inserted at head), so test that
  for (i=9; i>0; i--) {
    uint8_t* result = test_list.iterate();

    ASSERT_EQ(i, *result);
  }
}

TEST_F(LinkedListTest, TestInsertAndDelete) {
  uint8_t i = 0;
  uint8_t*  result = &i; // initialise it with something

  // insert 0 to 9
  for (i=0; i<=9; i++) {
    test_list.insert(i);
  }

  for (i=0; i<=9; i++) {
    test_list.remove(i);
  }

  for (i=0; i<=2; i++) {
    result = test_list.iterate();
    ASSERT_EQ(result, nullptr);
  }
}