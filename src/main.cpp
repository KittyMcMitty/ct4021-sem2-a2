#ifndef UNIT_TEST // Don't run this file in test scenario

#include "ArduinoInterface.h"
#include <LiquidCrystal.h>
#include <radar.h>
#include <MyLED.h>
#include <CommandQueue.h>
#include <avr/pgmspace.h> // PROGMEM macro

void setup() {

  Serial.begin(9600);

}

void loop() {

}


#endif // UNIT_TEST