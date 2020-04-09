#ifndef UNIT_TEST // Don't run this file in test scenario

#include "ArduinoInterface.h"
#include <LiquidCrystal.h>
#include "radar.h"
#include <MyLED.h>

// refactor this later to use #define. Does it use more memory? Does compiler optimise away?

// Radar pins
const uint8_t trigger_pin = 11, echo_pin = 12, servo_pin =  9;

// LCD pins
const uint8_t d4 =  5, d5 =  4, d6 =  3, d7 =  2, rs =  9, en =  9;

Radar<Servo, ConcreteArduino> radar;
uint32_t distance = 0;

MyLED my_led(0,1,2);

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(9600);
  radar.init(trigger_pin, echo_pin, servo_pin);
}

void loop()
{
  radar.move();
  distance = radar.ping();
  delay(10);
}

#endif // UNIT_TEST