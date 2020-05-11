//
// Created by Nicholas Ives on 11/05/2020.
//
#include <radar.h>

namespace EchoISR {

volatile uint32_t pulse_start_  {0};
volatile uint32_t pulse_end_    {0};
volatile bool i_flag {false};
uint8_t echo_pin_               {0};  // echo pin for sensor

void echo_isr() {
  using AI = ArduinoInterface;

  i_flag = true;

  // if the pin is set high
  if (AI::digitalRead(echo_pin_) == HIGH) {
  pulse_start_ = AI::micros(); // get the start time
  } else {
  pulse_end_ = AI::micros(); // else get the end time
  }
}

} // namespace EchoISR