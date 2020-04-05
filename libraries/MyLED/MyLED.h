//
// Created by Nicholas Ives on 28/03/2020.
//

#ifndef A_TOOLCHAIN_TEST_LIBRARIES_MYLED_MYLED_H_
#define A_TOOLCHAIN_TEST_LIBRARIES_MYLED_MYLED_H_

#include <ArduinoInterface.h>
#include <cstdint>

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
template<class T>
class MyLED  {
 private:
  uint8_t red_pin_    {0};
  uint8_t green_pin_  {0};
  uint8_t blue_pin_   {0};

  uint8_t   brightness_ {255};  // Used by pulse() to know current brightness
  int8_t   increment_  {5};    // Pulse increment

  LEDColour led_colour_ {LEDColour::GREEN};


 public:

  // Arguments are red, green, and blue pins for LED
  MyLED(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) {

    // save pins
    red_pin_ = red_pin;
    green_pin_ = green_pin;
    blue_pin_ = blue_pin;

    // Set the pins for output
    T::pinMode(red_pin_, OUTPUT);
    T::pinMode(green_pin_, OUTPUT);
    T::pinMode(blue_pin_, OUTPUT);
  };

  // set colour
  void set_colour(LEDColour led_colour) {

    led_colour_ = led_colour; // save colour

    // Red
    if (led_colour == LEDColour::RED) {
      T::digitalWrite(red_pin_, HIGH);
      T::digitalWrite(green_pin_, LOW);

      // Yellow
    } else if (led_colour == LEDColour::YELLOW) {
      T::digitalWrite(red_pin_, HIGH);
      T::digitalWrite(green_pin_, HIGH);

      // Green
    } else if (led_colour == LEDColour::GREEN) {
      T::digitalWrite(red_pin_, LOW);
      T::digitalWrite(green_pin_, HIGH);
    }
  };


  // Set LED pulse. Increment is how much brightness should vary per pulse call;
  inline void set_pulse(int8_t increment) { increment_ = increment; };

  void pulse() {

    // if we are at min or max brightness...
    if (brightness_ == 0 || brightness_ == 255) {
      increment_ *= -1; // reverse the sign of the increment
    }

    // TODO: Change this to use CPU status flags to check overflow
    // make sure we don't over/underflow
    // ref: Understanding Integer Overflow in C/C++; Stallings
    // Avoid mixing signed and unsigned §10.5.3 Stroustrup, so cast to uint8_t
    // if we are decrementing brightness
    if (increment_ < 0) {
      auto pos_inc = (uint8_t)abs(increment_); // increment in positive form

      // if brightness is greater than or equal to increment, subtract increment.
      // Otherwise subtract brightness (i.e. make it 0)
      brightness_ -= (brightness_ >= pos_inc) ? pos_inc : brightness_;


    } else { // increasing brightness

      //
      uint8_t difference = UINT8_MAX - brightness_;

      // If different is bigger than the increment, add the increment.
      // Otherwise add the difference.
      brightness_ += (difference >= increment_) ? increment_ : difference;

    }

    // This is the most likely path
    if (led_colour_ == LEDColour::GREEN) {
      T::analogueWrite(green_pin_, brightness_);

      // second most likely
    } else if (led_colour_ == LEDColour::RED) {
      T::analogueWrite(red_pin_, brightness_);

    } else { // Yellow, currently unused but here for completeness
      T::analogueWrite(red_pin_, brightness_);
      T::analogueWrite(green_pin_, brightness_);
    }
  };
};

#endif //A_TOOLCHAIN_TEST_LIBRARIES_MYLED_MYLED_H_
