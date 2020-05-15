//
// Created by Nicholas Ives on 13/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKCOMMANDQUEUE_H_
#define A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKCOMMANDQUEUE_H_

#include <gmock/gmock.h>
#include <FunctionObject.h>

class MockCommandQueue {
 public:
  MockCommandQueue();
  ~MockCommandQueue();
  MOCK_METHOD(void, add_entry, (FunctionObject*, uint16_t));
  MOCK_METHOD(void, remove_entry, (FunctionObject*));
  MOCK_METHOD(uint32_t, execute_current_entry, ());
  MOCK_METHOD(void, clear_queue, ());
};

class CommandQueueMockInterface  {
 public:
  inline static MockCommandQueue* mock_queue_;

  void add_entry(FunctionObject* function, uint16_t frequency) {
    mock_queue_->add_entry(function, frequency);
  }
  void remove_entry(FunctionObject* function) {
    mock_queue_->remove_entry(function);
  }
  uint32_t execute_current_entry() {
    return mock_queue_->execute_current_entry();
  }
  void clear_queue() {
    mock_queue_->clear_queue();
  }
};

#endif //A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKCOMMANDQUEUE_H_
