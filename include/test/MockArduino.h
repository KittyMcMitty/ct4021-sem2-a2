//
// Created by Nicholas Ives on 13/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKARDUINO_H_
#define A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKARDUINO_H_

#include <gmock/gmock.h>

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
  MockArduinoClass();
  ~MockArduinoClass();
  MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
  MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
  MOCK_METHOD(uint8_t , digitalRead, (uint8_t));
  MOCK_METHOD(void, delayMicroseconds, (unsigned int));
  MOCK_METHOD(unsigned long, pulseIn, (uint8_t, uint8_t));
  MOCK_METHOD(unsigned long, pulseIn, (uint8_t, uint8_t, uint32_t));
  MOCK_METHOD(void, analogueWrite, (uint8_t, uint8_t));
  MOCK_METHOD(uint32_t, millis, ());
  MOCK_METHOD(uint32_t, micros, ());
  MOCK_METHOD(void, attachInterrupt, (uint8_t, void (*)(), uint8_t));
  MOCK_METHOD(void, tone, (uint8_t, uint16_t, uint32_t duration));
  MOCK_METHOD(void, noTone, (uint8_t));
};


#endif //A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKARDUINO_H_
