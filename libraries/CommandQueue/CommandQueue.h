//
// Created by Nicholas Ives on 29/03/2020.
//

#ifndef A_TOOLCHAIN_TEST_LIBRARIES_COMMANDQUEUE_COMMANDQUEUE_H_
#define A_TOOLCHAIN_TEST_LIBRARIES_COMMANDQUEUE_COMMANDQUEUE_H_

#include <ArduinoInterface.h>
#include <LinkedList.h>
#include <FunctionObject.h>
//#include <Commands.h>

/*
 * CommandQueueEntry - an entry for the Command Queue
 *
 * A command queue entry consists of three parts - a function pointer to be
 * executed, the last time it was called, and how frequently it should be
 * called. This class is a friend to CommandQueue, so CommandQueue has access
 * to its internal state.
 *
 * This class also defined comparison operators. A CommandQueueEntry is equal to
 * another CommmandQueueEntry if they have the same function pointer, and is
 * greater than or lesser than another CommandQueueEntry by comparing the last
 * call time plus the frequency.
 */
class CommandQueueEntry {
  friend class CommandQueue;

 private:
  FunctionObject* function_;
  //void (*function_)() {nullptr};  // function to be called
  uint32_t  last_call_ {UINT32_MAX};       // time last called as returned by millis()
  uint16_t  frequency_ {UINT16_MAX};        // how many ms desired between calls

 public:
  CommandQueueEntry() = default;

  CommandQueueEntry(
      FunctionObject* function,
      uint32_t last_call,
      uint8_t frequency
      ) : function_ {function}, last_call_{last_call}, frequency_{frequency} {};

  // comparison operators
  bool operator==(const CommandQueueEntry& rhs) const;
  bool operator!=(const CommandQueueEntry& rhs) const;
  bool operator<(const CommandQueueEntry& rhs) const;
  bool operator<=(const CommandQueueEntry& rhs) const;
  bool operator>(const CommandQueueEntry& rhs) const;
  bool operator>=(const CommandQueueEntry& rhs) const;
};

/*
 * CommandQueue - a queue of commands to execute
 *
 * This class extends LinkedList and provides methods
 */
class CommandQueue {
 private:
  LinkedList<CommandQueueEntry> queue_;
  CommandQueueEntry* current_command {nullptr};

  //uint32_t command_calls_ {0};

  void update_current_command_();

 public:

  void add_entry(FunctionObject* function, uint16_t frequency);
  void remove_entry(FunctionObject* function);
  void clear_queue();
  uint32_t execute_current_entry();


};


#endif //A_TOOLCHAIN_TEST_LIBRARIES_EVENTHANDLER_COMMANDQUEUE_H_
