//
// Created by Nicholas Ives on 25/03/2020.
//

#include "SomeClass.h"

SomeClass::SomeClass(uint8_t num) {
  num_ = num;
}

void SomeClass::set_num(uint8_t num) {
  num_ = num;
}

uint8_t SomeClass::increment() {
  return ++num_;
}

