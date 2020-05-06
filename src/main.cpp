#ifndef UNIT_TEST // Don't run this file in test scenario

#include "ArduinoInterface.h"
#include <LiquidCrystal.h>
#include "radar.h"
#include <MyLED.h>
#include <CommandQueue.h>
#include <avr/pgmspace.h> // PROGMEM macro

namespace CT4009a2 {
namespace CFG {

// PROGMEM macro stores these constants on flash rather than in SRAM
// Radar pins
const uint8_t trigger_pin PROGMEM = A1, echo_pin = 3,
    servo_pin PROGMEM = 9;

// LCD pins
const uint8_t d4 PROGMEM = 8, d5 PROGMEM = 7, d6 PROGMEM = 4, d7 PROGMEM = 2,
    rs PROGMEM = 13, en PROGMEM = 12;

// IR sensor
const uint8_t ir_pin PROGMEM = 11;

// LED pins
const uint8_t red_pin PROGMEM = 10, green_pin PROGMEM = 6, blue_pin PROGMEM = 5;
} // namespace CFG

Radar<Servo> radar;
MyLED my_led(CFG::red_pin, CFG::green_pin, CFG::blue_pin);
LiquidCrystal lcd(CFG::rs, CFG::en, CFG::d4, CFG::d5, CFG::d6, CFG::d7);
CommandQueue queue;

// forward declare all this stuff. Maybe move all this into seperate file?
void move_radar();
void standby_pulse_led();
void sense_distance();
void init_mode_sensing();
void init_mode_standby();


void move_radar() {
  radar.move();
}

void standby_pulse_led() {
  my_led.pulse();
  uint8_t pir_sensor = digitalRead(CFG::ir_pin);

  if (pir_sensor) {
    init_mode_sensing();
  }
}

void sense_distance() {
  uint32_t distance = radar.ping();

  lcd.setCursor(0,1);
  lcd.print("D: ");
  lcd.print(distance);

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
  queue.add_entry(move_radar, 50);
  queue.add_entry(sense_distance, 550);

  my_led.set_pulse(200);

}

void init_mode_standby() {
  //queue.add_entry(move_radar, 30);
  //queue.remove_entry(sense_distance);
  my_led.set_colour(LEDColour::GREEN);
  my_led.set_pulse(6);
  queue.add_entry(standby_pulse_led, 20);
}

} // namespace CT4009a2


void setup() {
  using CT4009a2::radar;
  using CT4009a2::lcd;

  Serial.begin(9600);
  radar.init(CT4009a2::CFG::trigger_pin, CT4009a2::CFG::echo_pin, CT4009a2::CFG::servo_pin);
  lcd.begin(16,2);

  CT4009a2::init_mode_standby();
  lcd.clear();
}

uint32_t loop_count {0};
uint16_t i_call {0};

void loop() {

  using CT4009a2::queue;
  using CT4009a2::lcd;

  uint32_t next_time = queue.execute_current_entry();
  uint32_t current_time = millis();

  // if current time is less than next time, just do the next command immediately
  next_time -= (current_time < next_time) ? current_time : next_time;

  if (EchoISR::i_flag) {
    ++i_call;
    EchoISR::i_flag = false;
  }

  ++loop_count;
  lcd.setCursor(0,0);
  lcd.print("L:");
  lcd.print(loop_count);
  lcd.print("I:");
  lcd.print(i_call);

  /*uint8_t input = digitalRead(CT4009a2::CFG::ir_pin);

  if (input == 0) {
    lcd.print("false: ");
    lcd.print(input);
  } else {
    lcd.print("true:");
    lcd.print(input);
  } */

  lcd.print("N:");
  lcd.print(next_time);
  next_time -= (next_time!=0) ? 1 : 0;

  // delay 1ms less than the next time (unless next time is 0) so we're never late
  delay(next_time);
}


#endif // UNIT_TEST