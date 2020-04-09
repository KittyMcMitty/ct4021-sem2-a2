//
// Created by Nicholas Ives on 28/03/2020.
//

#ifndef A_TOOLCHAIN_TEST_LIBRARIES_MYLED_MYLED_H_
#define A_TOOLCHAIN_TEST_LIBRARIES_MYLED_MYLED_H_

#include <ArduinoInterface.h>

enum class LEDColour { RED, YELLOW, GREEN };

/*
 * MyLED - Simple RGB LED class.
 *
 * Initilise with the red, green, and blue pins used.
 *
 * Provides three methods - set_colour(), set_pulse(), and pulse().
 *
 * set_colour sets the current LED colour using an LEDColour enum class.
 * set_pulse set the LED pulse duration in ms.
 * pulse pulses
 *
 * Template parameter is concrete or mock Arduino interface
 */
//template<class T>
class MyLED {
 private:
  uint8_t red_pin_{0};
  uint8_t green_pin_{0};
  uint8_t blue_pin_{0};

  uint8_t brightness_{255};  // Used by pulse() to know current brightness
  int8_t increment_{5};    // Pulse increment

  LEDColour led_colour_{LEDColour::GREEN};

 public:

  // Arguments are red, green, and blue pins for LED
  MyLED(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);
  // set colour
  void set_colour(LEDColour led_colour);

  // Set LED pulse. Increment is how much brightness should vary per pulse call;
  inline void set_pulse(int8_t increment) { increment_ = increment; };

  void pulse();
};

#endif //A_TOOLCHAIN_TEST_LIBRARIES_MYLED_MYLED_H_
