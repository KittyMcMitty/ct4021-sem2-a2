#ifndef UNIT_TEST // Don't run this file in test scenario

#include "ArduinoInterface.h"
#include <LiquidCrystal.h>
#include "radar.h"
#include <MyLED.h>
#include <CommandQueue.h>
#include <avr/pgmspace.h> // PROGMEM macro

namespace CT4009a2 {
namespace CFG {
// refactor this later to use #define. Does it use more memory? Does compiler optimise away?
// PROGMEM macro stores these constants on flash rather than in SRAM

// Radar pins
const uint8_t trigger_pin PROGMEM = 11, echo_pin PROGMEM = 12,
    servo_pin PROGMEM = 9;

// LCD pins
const uint8_t d4 PROGMEM = 5, d5 PROGMEM = 4, d6 PROGMEM = 3, d7 PROGMEM = 2,
    rs PROGMEM = 10, en PROGMEM = 10;

// IR sensor
const uint8_t ir_pin PROGMEM = 7;

// LED pins
const uint8_t red_pin PROGMEM = 3, green_pin PROGMEM = 5, blue_pin PROGMEM = 6;
} // namespace CFG

volatile bool ir_trigger = false; // volatile as changed by ISR

Radar<Servo> radar;
MyLED my_led(CFG::red_pin, CFG::green_pin, CFG::blue_pin);
LiquidCrystal lcd(CFG::rs, CFG::en, CFG::d4, CFG::d5, CFG::d6, CFG::d7);
CommandQueue queue;

// forward declare all this stuff. Maybe move all this into seperate file?
void move_radar();
void standby_pulse_led();
void pir_interrupt_callback();
void sense_distance();
void init_mode_sensing();
void init_mode_standby();

void move_radar() {
  radar.move();
}

void standby_pulse_led() {
  my_led.pulse();
  if (ir_trigger == true) {
    init_mode_sensing();
  }
}

void pir_interrupt_callback() {
  ir_trigger = true;
}

void sense_distance() {
  uint32_t distance = radar.ping();

  if (distance < 500) {
    my_led.set_colour(LEDColour::RED);
  } else if (distance < 1500) {
    my_led.set_colour(LEDColour::YELLOW);
  } else {
    my_led.set_colour(LEDColour::GREEN);
  }
}

void init_mode_sensing() {
  queue.remove_entry(standby_pulse_led);
  queue.add_entry(move_radar, 15);
  queue.add_entry(sense_distance, 100);
  detachInterrupt(digitalPinToInterrupt(CFG::ir_pin));
}

void init_mode_standby() {
  queue.remove_entry(move_radar);
  queue.remove_entry(sense_distance);
  my_led.set_colour(LEDColour::GREEN);
  my_led.set_pulse(51);
  queue.add_entry(standby_pulse_led, 200);
  attachInterrupt(digitalPinToInterrupt(CFG::ir_pin), pir_interrupt_callback, RISING);
}


} // namespace CT4009a2
void setup() {
  using CT4009a2::radar;
  Serial.begin(9600);
  radar.init(CT4009a2::CFG::trigger_pin, CT4009a2::CFG::echo_pin, CT4009a2::CFG::servo_pin);
  CT4009a2::init_mode_standby();
}

void loop() {
  using CT4009a2::queue;
  uint32_t time_to_wait = queue.execute_current_entry();

  delay(time_to_wait);
}


#endif // UNIT_TEST