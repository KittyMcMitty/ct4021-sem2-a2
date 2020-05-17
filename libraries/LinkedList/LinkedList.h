//
// Created by Nicholas Ives on 06/04/2020.
//

#ifndef A_TOOLCHAIN_TEST_LIBRARIES_LINKEDLIST_LINKEDLIST_H_
#define A_TOOLCHAIN_TEST_LIBRARIES_LINKEDLIST_LINKEDLIST_H_

#ifdef UNIT_TEST
#include <cstdint>
#else
#include <ArduinoInterface.h>
//#include <new.h>
#include <stdlib.h>
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

  class Iterator {
   private:
    ListNode<T>* node_ {nullptr};

   public:
    explicit Iterator(ListNode<T>* n) {
      node_ = n;
    };

    /*
     * == Operator
     *
     * Just test equality of both pointers
     */
    inline bool operator==(const Iterator& rhs) const {
      return this->node_ == rhs.node_;
    };

    /*
     * != Operator
     */
    inline bool operator!=(const Iterator& rhs) const {
      return !this->operator==(rhs);
    };

    /*
     * Pre-increment operator
     */
    inline Iterator& operator++() {
      node_ = node_->next_;
      return *this;
    };

    /*
     * Post-increment operator
     */
    inline const Iterator operator++(int) const {
      auto tmp_node = node_;
      ++*this;
      return tmp_node;
    };

    /*
     * * Operator
     *
     * This returns the data referenced by the iterator
     */
    inline T* operator*() {
      return &node_->data_;
    };
  };

 private:
  ListNode<T>* head_{nullptr};

 public:
  ~LinkedList();
  void insert(T data);
  void remove(T data);


  Iterator begin() { return Iterator(head_); };
  Iterator end() { return Iterator(nullptr); };

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
    //free(old_ptr);
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

      } else { // snip current entry out of the list
        old_ptr->next_ = current_ptr->next_;
        delete current_ptr;
        break;
      }
    } else { // carry on looking
      old_ptr = current_ptr;
      current_ptr = current_ptr->next_;
    }
  }
}

#endif //A_TOOLCHAIN_TEST_LIBRARIES_LINKEDLIST_LINKEDLIST_H_
