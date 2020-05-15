#ifndef UNIT_TEST // Don't run this file in test scenario

#include <ArduinoInterface.h>
#include <RadarState.h>
#include <LiquidCrystal.h>

RadarContext* context;

//LiquidCrystal lcd {CFG::rs, CFG::en, CFG::d0, CFG::d1, CFG::d2,
//                    CFG::d3, CFG::d4, CFG::d5, CFG::d6, CFG::d7};

void setup() {
  Serial.begin(9600);
  context = new RadarContext;
  context->init();
  //ArduinoInterface::pinMode(11, INPUT);
  //lcd.begin(16,2);
}

uint32_t count {0};

void loop() { /*
  lcd.setCursor(0,0);
  uint8_t  input = ArduinoInterface::digitalRead(11);
  lcd.print("Count: ");
  lcd.print(count);
  lcd.setCursor(0,1);
  lcd.print("Input: ");
  lcd.print(input);
  ++count;
  delay(100); */

uint32_t delay_time {0}, next_time {0};

  uint32_t current_time = millis();
  next_time = context->execute_current_entry();
  volatile uint32_t execution_time = millis() - current_time;

  next_time -= (current_time < next_time) ? current_time : next_time;



  delay(next_time);
}


#endif // UNIT_TEST