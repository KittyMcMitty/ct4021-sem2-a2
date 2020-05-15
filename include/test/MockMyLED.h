//
// Created by Nicholas Ives on 13/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKMYLED_H_
#define A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKMYLED_H_

#include <gmock/gmock.h>

enum class LEDColour { RED, YELLOW, GREEN };

class MockMyLED {
 public:
  MockMyLED();
  ~MockMyLED();
  MOCK_METHOD(void, MyLED, (uint8_t, uint8_t, uint8_t));
  MOCK_METHOD(void, set_colour, (LEDColour));
  MOCK_METHOD(void, set_pulse, (int8_t));
  MOCK_METHOD(void, pulse, ());
};

class MyLEDMockInterface {
 public:
  inline static MockMyLED* mock_led_;

  MyLEDMockInterface(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) {
    //mock_led_->MyLED(red_pin, green_pin, blue_pin);
  }

  void set_colour(LEDColour colour) {
    mock_led_->set_colour(colour);
  }
  void set_pulse(int8_t increment) {
    mock_led_->set_pulse(increment);
  }
  void pulse() {
    mock_led_->pulse();
  }
};

#endif //A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKMYLED_H_
