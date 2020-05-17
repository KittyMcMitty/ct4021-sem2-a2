#ifndef UNIT_TEST // Don't run this file in test scenario

#include <ArduinoInterface.h>
#include <RadarState.h>
#include <LiquidCrystal.h>

RadarContext* context;

void setup() {
  Serial.begin(9600);
  context = new RadarContext;
  context->init();
}

void loop() {

uint32_t next_time {0};

  next_time = context->execute_current_entry();
  uint32_t current_time = millis();

  // make sure we don't underflow next_time
  next_time -= (current_time < next_time) ? current_time : next_time;

  delay(next_time);
}


#endif // UNIT_TEST