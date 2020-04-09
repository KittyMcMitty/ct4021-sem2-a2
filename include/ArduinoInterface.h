//
// Created by Nicholas Ives on 27/03/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_ARDUINOINTERFACE_H_
#define A_TOOLCHAIN_TEST_INCLUDE_ARDUINOINTERFACE_H_

#ifdef UNIT_TEST
#include "gmock/gmock.h"

#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1

/*
 * MockArduinoClass.
 *
 * Contains mocks of all the Arduino methods used in this project.
 *
 * NOTE: This is NOT the mock class used with ArduinoInterface. This is because
 * GMock creates classes with virtual methods, which is problematic as
 * production code expects to be able to call these as free functions. So, this
 * class is used as a member of the MockArduino struct.
 */
class MockArduinoClass{
 public:
  MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
  MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
  MOCK_METHOD(void, delayMicroseconds, (unsigned int));
  MOCK_METHOD(unsigned long, pulseIn, (uint8_t, uint8_t));
  MOCK_METHOD(void, analogueWrite, (uint8_t, uint8_t));
  MOCK_METHOD(uint32_t, millis, ());
};

/*
 * MockArduino.
 *
 * Struct wrapper around MockArduinoClass. This allows the mock class comprised
 * of virtual methods to be used in code expecting static calls.
 *
 * https://stackoverflow.com/questions/8942330/google-mock-unit-testing-static-methods-c
 */
struct MockArduino {
  // inline here is C++17. Avoids linker errors.
  inline static MockArduinoClass *mock;

  static void pinMode(uint8_t pin, uint8_t mode) {
    return mock->pinMode(pin, mode);
  }

  static void digitalWrite(uint8_t pin, uint8_t val) {
    mock->digitalWrite(pin, val);
  };
  static void delayMicroseconds(unsigned int us) {
    mock->delayMicroseconds(us);
  };
  static unsigned long pulseIn(uint8_t pin, uint8_t val) {
    return mock->pulseIn(pin, val);
  }
  static void analogueWrite(uint8_t pin, uint8_t val) {
    return mock->analogueWrite(pin,val);
  }
  static uint32_t millis() {
    return mock->millis();
  }
};

#else
#include <Arduino.h>

/*
 * ConcreteArduino
 *
 * This class is just a static wrapper around the Arduino functions that are
 * used in this project
 */
class ConcreteArduino {
 public:
  inline static void pinMode(uint8_t pin, uint8_t mode) {
    ::pinMode(pin, mode);
  };
  inline static void digitalWrite(uint8_t pin, uint8_t val) {
    ::digitalWrite(pin, val);
  };
  inline static void delayMicroseconds(unsigned int us) {
    ::delayMicroseconds(us);
  };
  inline static unsigned long pulseIn(uint8_t pin, uint8_t val) {
    return ::pulseIn(pin, val);
  }
  inline static void analogueWrite(uint8_t pin, uint8_t val) {
    return ::analogWrite(pin, val);
  }
  inline static uint32_t millis() {
    return ::millis();
  }
};

#endif // UNIT_TEST

/*
 * ArduinoInterface template class.
 *
 * This class allows us to swap between our mock Arduino interface and the real
 * one. As both this and the concrete class are just a bunch of inline static
 * methods, they will get optimised away in the release build.
 */
class ArduinoInterface {
 public:
#ifdef UNIT_TEST // this avoids having to template every class using this interface
 using AI = MockArduino;
#else
  using AI = ConcreteArduino;
#endif // UNIT_TEST
  inline static void pinMode(uint8_t pin, uint8_t mode) {
    AI::pinMode(pin, mode);
  }
  inline static void digitalWrite(uint8_t pin, uint8_t val) {
    AI::digitalWrite(pin, val);
  };
  inline static void delayMicroseconds(unsigned int us) {
    AI::delayMicroseconds(us);
  };
  inline static unsigned long pulseIn(uint8_t pin, uint8_t val) {
    return AI::pulseIn(pin, val);
  }
  inline static void analogueWrite(uint8_t pin, uint8_t val) {
    return AI::analogueWrite(pin, val);
  }
  inline static uint32_t millis() {
    return AI::millis();
  }
};

#endif //A_TOOLCHAIN_TEST_INCLUDE_ARDUINOINTERFACE_H_
