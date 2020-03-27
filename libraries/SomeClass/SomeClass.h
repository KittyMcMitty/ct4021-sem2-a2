//
// Created by Nicholas Ives on 25/03/2020.
//

#ifndef A_TOOLCHAIN_TEST_LIB_SOMECLASS_H_
#define A_TOOLCHAIN_TEST_LIB_SOMECLASS_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif

class SomeClass {
 public:
  explicit SomeClass(uint8_t num);
  ~SomeClass() = default;
  virtual void set_num(uint8_t num);
  virtual uint8_t increment();

 private:
  uint8_t num_;
};

#endif //A_TOOLCHAIN_TEST_LIB_SOMECLASS_H_
