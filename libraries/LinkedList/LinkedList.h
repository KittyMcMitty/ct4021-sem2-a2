//
// Created by Nicholas Ives on 06/04/2020.
//

#ifndef A_TOOLCHAIN_TEST_LIBRARIES_LINKEDLIST_LINKEDLIST_H_
#define A_TOOLCHAIN_TEST_LIBRARIES_LINKEDLIST_LINKEDLIST_H_

#ifdef UNIT_TEST
#include <cstdint>
#else
#include <ArduinoInterface.h>
#endif // UNIT_TEST

template <typename T> class ListNode;
template <typename T> class LinkedList;

template<typename T>
class ListNode {
  friend class LinkedList<T>;
 private:
  T data_;
  ListNode<T>* next_ {nullptr};

 public:
  ListNode(T data, ListNode<T> *next) {
    data_ = data;
    next_ = next;
  };
};

/*
 * LinkedList - a simple linked list class
 *
 * This class provides an insert, delete, and iterate method. Insert will place
 * an entry at the head of the list. Delete will delete the first found match in
 * the list. Iterate returns a subsequent list entry with every call, and NULL
 * to indicate the end of the list.
 */
template<typename T>
class LinkedList {

 private:
  ListNode<T> *head_{nullptr};
  ListNode<T> *tail_{nullptr};
  ListNode<T> *iterate_ptr_{head_}; //used by iterator methods / operators

 public:
  ~LinkedList();
  void insert(T data);
  void remove(T data);
  T iterate();
  void reset_iterate();

  /*
   * Iterator - iterator helper class
   *
   * This class provides methods to iterate over the linked list. As this is a
   * singly linked list, there is no random access and only forward traversal.
   */
  class iterator {
   private:
    LinkedList<T>* list_;

   public:
    explicit iterator(LinkedList<T>* list) : list_(list) {}

    T operator*() const { return list_->data; }

    iterator operator++();    // pre-increment
    iterator operator++(int); // post-increment

  };

  iterator begin()  { return iterator(this); } // an iterator for this list
  iterator end()    { return iterator(nullptr); } // end is nullptr


};

/*
 * ~LinkedList - Destructor
 *
 * This destructor traverses the list and delete's every node.
 */
template<typename T>
LinkedList<T>::~LinkedList() {

  ListNode<T> *current_ptr = head_;
  ListNode<T> *old_ptr = nullptr;

  // while current pointer points to something...
  while (current_ptr!=nullptr) {
    old_ptr = current_ptr; // save that pointer
    current_ptr = current_ptr->next_; // advance to next pointer
    delete old_ptr; // and now delete that object
  }
}

/*
 * Insert - add an item to the list
 *
 * This will stick an item at the START of the list. This is makes this nice
 * and fast. So, items should be added in REVERSE order as the things you add
 * later will be return before, if that is important to you.
 */
template<typename T>
void LinkedList<T>::insert(T data) {

  // if list is not empty (more common case)
  if (head_ != nullptr) {
    auto node = new ListNode<T>(data, head_);
    head_ = node;

  } else {
    auto node = new ListNode<T>(data, nullptr);
    head_ = node;
    //iterate_ptr_ = node; // stop iterate_ptr_ from being null
  }
}


/*
 * Remove - remove an entry from the list.
 *
 * This method will iterate over the list and remove the first matching
 * entry found. If the same thing has been added twice, it will only be
 * deleted once!
 */
template<typename T>
void LinkedList<T>::remove(T data) {

  ListNode<T>* current_ptr = head_;
  ListNode<T>* old_ptr = nullptr;

  // while we're not at the end
  while (current_ptr !=nullptr) {
    // if we find a match..
    if (current_ptr->data_ == data) {
      if (old_ptr == nullptr) { // this is true if we're still at head
        head_ = current_ptr->next_; // make head the next one
        delete current_ptr;
        break; // break out of loop here

      } else {
        old_ptr->next_ = current_ptr->next_;
        delete current_ptr;
        break;
      }
    } else {
      old_ptr = current_ptr;
      current_ptr = current_ptr->next_;
    }
  }
}


/*
 * Iterate - iterates over the list
 *
 * This will return the data from the current list node and then advance
 * iterate_ptr to the next node in the list. If iterate_ptr is nullptr, reset
 * iterate_ptr back to head_ and return null so the caller knows the end
 * is nigh.
 *
 * Of note: items added to the list while it is iterating won't be returned
 * until the next iteration round, as they are inserted at the head.
 */
template<typename T>
T LinkedList<T>::iterate() {
  // if we aren't at the end of the list
  if (iterate_ptr_ != nullptr) {
    ListNode<T>* old_ptr = iterate_ptr_;  // save current pointer
    iterate_ptr_ = iterate_ptr_->next_;   // advance iterator
    return old_ptr->data_;                // return saved data

  } else {
    iterate_ptr_ = head_; // reset iterator back to head
    T empty;
    return empty; // return an empty object
  }
}

template<typename T>
void LinkedList<T>::reset_iterate() {
  iterate_ptr_ = head_;
}

#endif //A_TOOLCHAIN_TEST_LIBRARIES_LINKEDLIST_LINKEDLIST_H_
