#include <Arduino.h>
#include <SomeClass.h>

SomeClass my_class(1);

void setup() {
  my_class.set_num(2);
}

void loop() {
  uint8_t num = my_class.increment();
}