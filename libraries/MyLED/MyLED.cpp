//
// Created by Nicholas Ives on 28/03/2020.
//

#include <MyLED.h>

MyLED::MyLED(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) :
        red_pin_{red_pin}, green_pin_{green_pin}, blue_pin_{blue_pin} {

  using AI = ArduinoInterface;

  // Set the pins for output
  AI::pinMode(red_pin_, OUTPUT);
  AI::pinMode(green_pin_, OUTPUT);
  AI::pinMode(blue_pin_, OUTPUT);
};

void MyLED::set_colour(LEDColour led_colour) {

  using AI = ArduinoInterface;

  // only do this if colour is changing
  if (led_colour_ != led_colour) {
    led_colour_ = led_colour; // save colour
    brightness_ = UINT8_MAX; // set brightness to max

    // Red
    if (led_colour == LEDColour::RED) {
      AI::digitalWrite(red_pin_, HIGH);
      AI::digitalWrite(green_pin_, LOW);

      // Yellow
    } else if (led_colour == LEDColour::YELLOW) {
      AI::digitalWrite(red_pin_, HIGH);
      AI::digitalWrite(green_pin_, HIGH);

      // Green
    } else if (led_colour == LEDColour::GREEN) {
      AI::digitalWrite(red_pin_, LOW);
      AI::digitalWrite(green_pin_, HIGH);
    }
  }
}

void MyLED::pulse() {

  using AI = ArduinoInterface;

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

    // difference between maximum and current brightness
    uint8_t difference = UINT8_MAX - brightness_;

    // If different is bigger than the increment, add the increment.
    // Otherwise add the difference.
    brightness_ += (difference >= increment_) ? increment_ : difference;

  }

  // This is the most likely path
  if (led_colour_ == LEDColour::GREEN) {
    AI::analogueWrite(green_pin_, brightness_);

    // second most likely
  } else if (led_colour_ == LEDColour::RED) {
    AI::analogueWrite(red_pin_, brightness_);

  } else { // Yellow, currently unused but here for completeness
    AI::analogueWrite(red_pin_, brightness_);
    AI::analogueWrite(green_pin_, brightness_);
  }
}
