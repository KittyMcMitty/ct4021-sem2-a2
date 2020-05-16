//
// Created by Nicholas Ives on 08/05/2020.
//


#ifndef A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_
#define A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_

#ifdef UNIT_TEST
#include <test/MockCommandQueue.h>
#include <test/MockMyLED.h>
#include <test/MockLiquidCrystal.h>
#include <test/MockRadar.h>
#else
#include <avr/pgmspace.h>
#include <CommandQueue.h>
#include <LiquidCrystal.h>
#include <radar.h>
#include <MyLED.h>
#endif // UNIT_TEST
#include <ArduinoInterface.h>





namespace CFG {

// PROGMEM macro stores these constants on flash rather than in SRAM
// Radar pins
const uint8_t trigger_pin PROGMEM = 4, echo_pin = 2,
    servo_pin PROGMEM = 9;

// LCD pins
const uint8_t d0 PROGMEM = A2, d1 PROGMEM = A3, d2 PROGMEM = A4,
    d3 PROGMEM = A5, d4 PROGMEM = 13, d5 PROGMEM = 12, d6 PROGMEM = 8,
    d7 PROGMEM = 7, rs PROGMEM = A0,  en PROGMEM = A1;

// IR sensor
const uint8_t ir_pin PROGMEM = 11;

// LED pins
const uint8_t red_pin PROGMEM = 3, green_pin PROGMEM = 6, blue_pin PROGMEM = 5;

const uint8_t buzzer_pin PROGMEM = 10;

// distance thresholds for radar
const uint32_t  distance_warning PROGMEM {60}, distance_red PROGMEM {300},
    distance_yellow PROGMEM {600}, distance_green PROGMEM {900};

} // namespace CFG


class RadarContext;
class RadarState;
class StandbyState;
class SensingState;
class WarningState;

class RadarContext {

 private:
  friend class RadarState;

#ifdef UNIT_TEST
  friend class RadarContextTest;
  MyLEDMockInterface led_;
  LiquidCrystalMockInterface lcd_;
  RadarMockInterface radar_;
  CommandQueueMockInterface queue_;
#else
  Radar<Servo>  radar_;
  CommandQueue  queue_;
  MyLED         led_ {CFG::red_pin, CFG::green_pin, CFG::blue_pin};
  LiquidCrystal lcd_ {CFG::rs, CFG::en, CFG::d0, CFG::d1, CFG::d2,
                      CFG::d3, CFG::d4, CFG::d5, CFG::d6, CFG::d7};
#endif // UNIT_TEST

  RadarState* state_ {nullptr};
  uint32_t timer_ {0}; // track how long since measurement in range

  TEST_VIRTUAL void change_state(RadarState* s); //
  TEST_VIRTUAL void command_add_entry(FunctionObject *func, uint16_t frequency);
  TEST_VIRTUAL void command_remove_entry(FunctionObject *func);
  TEST_VIRTUAL void led_set_colour(LEDColour colour);
  TEST_VIRTUAL void led_set_pulse(int8_t  increment);
  TEST_VIRTUAL uint32_t get_timer() const;
  TEST_VIRTUAL void set_timer();

 public:
  explicit RadarContext(RadarState* state = nullptr);
  TEST_VIRTUAL ~RadarContext();
  TEST_VIRTUAL void init();
  TEST_VIRTUAL void radar_move();
  TEST_VIRTUAL uint32_t radar_ping();
  TEST_VIRTUAL void led_pulse();
  TEST_VIRTUAL void lcd_setCursor(uint8_t col, uint8_t row);
  TEST_VIRTUAL void lcd_print(const char str[]);
  TEST_VIRTUAL void lcd_print(int n);
  TEST_VIRTUAL uint32_t execute_current_entry();

  TEST_VIRTUAL void start();
  void stop();
  TEST_VIRTUAL void update(uint32_t);
};


class RadarState {
 protected:
  static void change_state(RadarContext* c, RadarState* s);
  static void command_add_entry(RadarContext *c,
                                FunctionObject *func,
                                uint16_t frequency);
  static void command_remove_entry(RadarContext *c, FunctionObject *func);
  static void led_set_colour(RadarContext* c, LEDColour colour);
  static void led_set_pulse(RadarContext* c, int8_t  increment);
  static uint32_t get_timer(RadarContext* c);
  static void set_timer(RadarContext *c);

  RadarState() = default;

 public:
  virtual ~RadarState() = default;

  virtual void start(RadarContext *c) = 0;
  virtual void update(RadarContext *c, uint32_t input) = 0;
};


class StandbyState : public RadarState {

 private:
  StandbyState() = default;
  inline static RadarState* instance_ {nullptr};
  ~StandbyState() final = default;

 public:

  static RadarState* instance();
  static void delete_instance();
  void start(RadarContext *c) final;
  void update(RadarContext *c, uint32_t input) final;
};

class SensingState : public RadarState {
 private:
  inline static RadarState* instance_ {nullptr};
  SensingState() = default;
  ~SensingState() final = default;

  static void change_standby(RadarContext* c);
  static void change_warning(RadarContext* c);

 public:

  static RadarState* instance();
  static void delete_instance();
  void start(RadarContext *c) final;
  void update(RadarContext *c, uint32_t input) final;
};

class WarningState : public RadarState {
 private:
  inline static RadarState* instance_ {nullptr};
  WarningState() = default;
  ~WarningState() final = default;

 public:

  static RadarState* instance();
  static void delete_instance();
  void start(RadarContext *c) final;
  void update(RadarContext *c, uint32_t input) final;
};
#endif //A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_
