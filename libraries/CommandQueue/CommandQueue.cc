//
// Created by Nicholas Ives on 29/03/2020.
//


#include <ArduinoInterface.h>
#include <CommandQueue.h>

// TODO: Concept - use a linked list to store a collection of function pointers
// TODO: Rename this class CommandQueue. New concept is that class will execute
//  current command, then iterate list to find next command to execute. It will
//  then sleep until that command. That command will execute on wake, and so on.

// We can't pass around pointers to member functions, so use lambda's here to do
// the logic? Functoids probably too heavyweight

/*
 * Add Entry - add an entry to the command queue
 *
 * This method takes a function pointer and a frequency in ms. This class will
 * then call the function at the specified frequency. When a command is added,
 * it is scheduled to be executed next.
 *
 * If multiple commands are added, then the order of execution is undefined. If
 * you want to add and execute commands in sequence, interleave each add with a
 * call to execute_current_entry().
 */
void CommandQueue::add_entry(FunctionObject* function, uint16_t frequency) {

  using AI = ArduinoInterface;

  CommandQueueEntry command;
  command.function_ = function;
  command.frequency_ = frequency;

  command.last_call_ = AI::millis();
  /*
  // prevent underflow
  uint32_t time = AI::millis();
  if (time > frequency) {
    command.last_call_ = time - frequency; // makes it due to be called
  } else {
    command.last_call_ = 0;
  } */


  queue_.insert(command);
}

//
void CommandQueue::remove_entry(FunctionObject* function) {

  // make a queue entry
  CommandQueueEntry entry;

  // give it the function
  entry.function_ = function;

  // now we can compare it to existing entries for deletion
  queue_.remove(entry);

  // if we just deleted the current command, set it to nullptr
  current_command = nullptr;
}

/*
 * Execute Current Entry - executes the current command, return time to next
 *
 * This method will execute the current command and then update current command
 * to get entry ready for next call.
 */
uint32_t CommandQueue::execute_current_entry() {

  using AI = ArduinoInterface;

  // if we don't have a command, get one
  if (current_command == nullptr) {
    update_current_command_();
  }

  // if we have a command, do it and update
  if (current_command != nullptr) {

    // do the command
    (*current_command->function_)();
   // ++command_calls_;


    uint32_t current_time = AI::millis();

    // check we didn't change states in previous command. Dereferencing nullptr
    // is undefined, so don't do it!
    if (current_command !=nullptr) {
      // save the current time so we know when we called it
      current_command->last_call_ = current_time;
    }


    // get the next command ready for next time we are called
    update_current_command_();
    /*
    if (current_time >= (current_command->last_call_ + (uint32_t)current_command->frequency_)) {
      execute_current_entry();
    }
*/
    // last_call_ + frequency_ is the time the command should be called next
    return current_command->last_call_ + (uint32_t)current_command->frequency_;
  } else {
    return 0; // otherwise, there must be no commands!
  }
}

/*
 * Update Current Command
 *
 * Traverse list to get the next command. This sets the class member
 * current_command_
 */
void CommandQueue::update_current_command_() {

  for (auto entry : queue_) {
    if (current_command != nullptr) {
      if (*entry < *current_command) {
        current_command = entry;
      }
    } else {
      current_command = entry;
    }

  }
}


void CommandQueue::clear_queue() {
  for (auto entry : queue_) {
    remove_entry(entry->function_);
  }
}

/*
 * == Operator
 *
 * When comparing equal or not equal, just compare the function
 */
bool CommandQueueEntry::operator==(const CommandQueueEntry& rhs) const {
  return this->function_ == rhs.function_;
}

/*
 * != Operator
 */
bool CommandQueueEntry::operator!=(const CommandQueueEntry& rhs) const {
  return !this->operator==(rhs);
}

/*
 * < Operator
 *
 * When comparing less than or greater than, compare the sum of last_call_ and
 * frequency_
 */
bool CommandQueueEntry::operator<(const CommandQueueEntry &rhs) const {
  return (this->last_call_ + this->frequency_) < (rhs.last_call_ + rhs.frequency_);
}

/*
 * > Operator
 *
 * Just swap the sides round and use less than.
 */
bool CommandQueueEntry::operator>(const CommandQueueEntry &rhs) const {
  return rhs.operator<(*this);
}

/*
 * <= Operator
 *
 * Not less-than or equal if it's greater than.
 */
bool CommandQueueEntry::operator<=(const CommandQueueEntry &rhs) const {
  return !this->operator>(rhs);
}

/*
 * >= Operator
 *
 * It's greater than or equal if it's not less than.
 */
bool CommandQueueEntry::operator>=(const CommandQueueEntry &rhs) const {
  return !this->operator<(rhs);
}
