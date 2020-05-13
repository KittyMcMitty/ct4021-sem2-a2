//
// Created by Nicholas Ives on 10/04/2020.
//

#include <gmock/gmock.h>
#include <CommandQueue.h>
#include <ArduinoInterface.h>

//void function_a();
//void function_b();
bool a_result = false;
bool b_result = false;

class FunctorA : public FunctionObject {
 public:
  void operator()() {
    a_result = true;
  }
};

class FunctorB : public FunctionObject {
 public:
  void operator()() {
    b_result = true;
  }
};

FunctorA function_a;
FunctorB function_b;

 class CommandQueueEntryTest : public ::testing::Test {
 protected:

  uint32_t call_time_a_ = 10;
  uint32_t call_time_b_ = 20;

  uint8_t frequency_a_ = 5;
  uint8_t frequency_b_ = 15;

  CommandQueueEntry command_a {&function_a, call_time_a_, frequency_a_};
  CommandQueueEntry command_b {&function_b, call_time_b_, frequency_b_};

};

class CommandQueueTest : public CommandQueueEntryTest {
 public:

  CommandQueue queue_;

 protected:
  MockArduinoClass mock_arduino_;
  CommandQueueTest() {
    MockArduino::mock = &mock_arduino_;
  }

  ~CommandQueueTest() {
    a_result = false;
    b_result = false;
  }
};


/*
void function_a() {
  a_result = true;
}

void function_b() {
  b_result = true;
}
*/

// CommandQueue tests

TEST_F(CommandQueueTest, TestAddAndExecute) {
  using ::testing::Return;

  uint32_t millis_time = 100; // time returned by mock millis()
  uint32_t returned_time = 0;

  EXPECT_CALL(mock_arduino_, millis())
      .Times(2)
      .WillRepeatedly(Return(millis_time));;

  queue_.add_entry(&function_a, frequency_a_);
  returned_time = queue_.execute_current_entry();

  ASSERT_EQ(a_result, true);
  ASSERT_EQ(returned_time, millis_time + frequency_a_);

}

TEST_F(CommandQueueTest, TestMultipleAddAndExecute) {
  using ::testing::Return;

  uint32_t millis_time_a = 100; // function a added with this time
  uint32_t millis_time_b = 200; // function b added with this
  uint32_t millis_time_c = 300; // both functions get this when they execute
  uint32_t returned_time = 0;

  EXPECT_CALL(mock_arduino_, millis())
      .Times(4);

  ON_CALL(mock_arduino_, millis())
      .WillByDefault(Return(millis_time_a));

  queue_.add_entry(&function_a, frequency_a_);

  ON_CALL(mock_arduino_, millis())
      .WillByDefault(Return(millis_time_b));

  queue_.add_entry(&function_b, frequency_b_);

  ON_CALL(mock_arduino_, millis())
      .WillByDefault(Return(millis_time_c));

  returned_time = queue_.execute_current_entry();

  ASSERT_EQ(a_result, true);
  ASSERT_EQ(returned_time, millis_time_b + frequency_b_);

  returned_time = queue_.execute_current_entry();

  ASSERT_EQ(b_result, true);
  ASSERT_EQ(returned_time, millis_time_c + frequency_a_);

}

TEST_F(CommandQueueTest, TestAddAndRemove) {
  using ::testing::Return;

  uint32_t millis_time = 100;
  uint32_t returned_time = 0;

  EXPECT_CALL(mock_arduino_, millis())
      .Times(4)
      .WillRepeatedly(Return(millis_time));

  queue_.add_entry(&function_a, frequency_a_);
  queue_.add_entry(&function_b, frequency_b_);

  queue_.remove_entry(&function_a);

  returned_time = queue_.execute_current_entry();
  ASSERT_EQ(returned_time, millis_time + frequency_b_);
  returned_time = queue_.execute_current_entry();
  ASSERT_EQ(returned_time, millis_time + frequency_b_);

  ASSERT_EQ(a_result, false);
  ASSERT_EQ(b_result, true);

}

// CommandQueueEntry comparison tests

TEST_F(CommandQueueEntryTest, TestCompareEqual) {
  ASSERT_EQ(command_a, command_a);
  ASSERT_EQ(command_b, command_b);
}

TEST_F(CommandQueueEntryTest, TestCompareNotEqual) {
  ASSERT_NE(command_a, command_b);
}

TEST_F(CommandQueueEntryTest, TestCompareLessThanOrEqual) {
  ASSERT_LE(command_a, command_b);
}

TEST_F(CommandQueueEntryTest, TestCompareLessThan) {
  ASSERT_LT(command_a, command_b);
}

TEST_F(CommandQueueEntryTest, TestCompareGreaterThanOrEqual) {
  ASSERT_GE(command_b, command_a);
}

TEST_F(CommandQueueEntryTest, TestCompareGreaterThan) {
  ASSERT_GT(command_b, command_a);
}
