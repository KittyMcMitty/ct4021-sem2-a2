//
// Created by Nicholas Ives on 09/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_RADARSTATEMOCKS_H_
#define A_TOOLCHAIN_TEST_INCLUDE_RADARSTATEMOCKS_H_
#include <gmock/gmock.h>
#include <FunctionObject.h>
//#include <LiquidCrystal.h>
//#include <MyLED.h> // LEDColour enum



enum class LEDColour { RED, YELLOW, GREEN };


class MockRadar {
 public:
  MOCK_METHOD(void, move, ());
  MOCK_METHOD(uint32_t, ping, ());
  MOCK_METHOD(void, init, (uint8_t, uint8_t, uint8_t));
};

class MockCommandQueue {
 public:
  MOCK_METHOD(void, add_entry, (FunctionObject*, uint16_t));
  MOCK_METHOD(void, remove_entry, (FunctionObject*));
  MOCK_METHOD(uint32_t, execute_current_entry, ());
};

class MockMyLED {
 public:
  MOCK_METHOD(void, MyLED, (uint8_t, uint8_t, uint8_t));
  MOCK_METHOD(void, set_colour, (LEDColour));
  MOCK_METHOD(void, set_pulse, (int8_t));
  MOCK_METHOD(void, pulse, ());
};

class MockLiquidCrystal {
 public:
  MOCK_METHOD(void, LiquidCrystal, (uint8_t, uint8_t, uint8_t, uint8_t,
      uint8_t, uint8_t));
  MOCK_METHOD(void, setCursor, (uint8_t , uint8_t));
  MOCK_METHOD(void, print, (const char *));
  MOCK_METHOD(void, print, (int));
  MOCK_METHOD(void, clear, ());
  MOCK_METHOD(void, begin, (uint8_t, uint8_t));
};

class RadarMockInterface {
 public:
  inline static MockRadar* mock_radar_;

  void move() {
    mock_radar_->move();
  }
  uint32_t ping() {
    return mock_radar_->ping();
  }
  void init(uint8_t trigger_pin, uint8_t echo_pin, uint8_t servo_pin) {
    mock_radar_->init(trigger_pin, echo_pin, servo_pin);
  }
};

class CommandQueueMockInterface  {
 public:
  inline static MockCommandQueue* mock_queue_;

  void add_entry(FunctionObject* function, uint16_t frequency) {
    mock_queue_->add_entry(function, frequency);
  }
  void remove_entry(FunctionObject* function) {
    mock_queue_->remove_entry(function);
  }
  uint32_t execute_current_entry() {
    return mock_queue_->execute_current_entry();
  }
};

class MyLEDMockInterface {
 public:
  inline static MockMyLED* mock_led_;

  MyLEDMockInterface(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) {
    mock_led_->MyLED(red_pin, green_pin, blue_pin);
  }

  void set_colour(LEDColour colour) {
    mock_led_->set_colour(colour);
  }
  void set_pulse(int8_t increment) {
    mock_led_->set_pulse(increment);
  }
  void pulse() {
    mock_led_->pulse();
  }
};

class LiquidCrystalMockInterface {
 public:
  inline static MockLiquidCrystal* mock_lcd_;

  LiquidCrystalMockInterface(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1,
      uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) {
    mock_lcd_->LiquidCrystal(rs, enable, d0, d1, d2, d3);
  }

  void setCursor(uint8_t col, uint8_t row) {
    mock_lcd_->setCursor(col, row);
  }
  void print(const char str[]) {
    mock_lcd_->print(str);
  }
  void print(int num) {
    mock_lcd_->print(num);
  }
  void clear() {
    mock_lcd_->clear();
  }
  void begin(uint8_t col, uint8_t row) {
    mock_lcd_->begin(col, row);
  }
};
#endif //A_TOOLCHAIN_TEST_INCLUDE_RADARSTATEMOCKS_H_
