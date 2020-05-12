//
// Created by Nicholas Ives on 08/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_
#define A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_

#ifdef UNIT_TEST
#include <cstdint>
#else
#include <avr/pgmspace.h>
#endif

#include <ArduinoInterface.h>

#ifndef UNIT_TEST
#include <CommandQueue.h>
#include <LiquidCrystal.h>
#include <radar.h>
#include <MyLED.h>
#else
#include <RadarStateMocks.h>
#endif



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
  RadarMockInterface radar_;
  CommandQueueMockInterface queue_;
  MyLEDMockInterface led_ {CFG::red_pin, CFG::green_pin, CFG::blue_pin};
  LiquidCrystalMockInterface lcd_ {CFG::rs, CFG::en, CFG::d4, CFG::d5, CFG::d6, CFG::d7};
#else
  Radar<Servo>  radar_;
  CommandQueue  queue_;
  MyLED         led_ {CFG::red_pin, CFG::green_pin, CFG::blue_pin};
  LiquidCrystal lcd_ {CFG::rs, CFG::en, CFG::d0, CFG::d1, CFG::d2,
                      CFG::d3, CFG::d4, CFG::d5, CFG::d6, CFG::d7};
#endif
  RadarState* state_ {nullptr};
  uint32_t timer_; // track how long since measurement in range

  void change_state(RadarState* s); //
  void command_add_entry(FunctionObject *func, uint16_t frequency);
  void command_remove_entry(FunctionObject *func);
  void led_set_colour(LEDColour colour);
  void led_set_pulse(int8_t  increment);
  uint32_t get_timer() const;
  void set_timer();

 public:
  RadarContext();
  ~RadarContext();
  void init();
  void radar_move();
  uint32_t radar_ping();
  void led_pulse();
  void lcd_setCursor(uint8_t col, uint8_t row);
  void lcd_print(const char str[]);
  void lcd_print(int n);
  uint32_t execute_current_entry();

  void start();
  void stop();
  void update(uint32_t);
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
  static void lcd_setCursor(RadarContext* c, uint8_t row, uint8_t col);
  static void lcd_print(RadarContext* c, const char str[]);
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
  inline static RadarState* instance_;

 public:
  ~StandbyState() override = default;
  static RadarState* instance();
  void start(RadarContext *c) override;
  void update(RadarContext *c, uint32_t input) override;
};

class SensingState : public RadarState {
 private:
  inline static RadarState* instance_;
  SensingState() = default;
  static void change_standby(RadarContext* c);
  static void change_warning(RadarContext* c);

 public:
  ~SensingState() override = default;
  static RadarState* instance();
  void start(RadarContext *c) override;
  void update(RadarContext *c, uint32_t input) override;
};

class WarningState : public RadarState {
 private:
  inline static RadarState* instance_;
  WarningState() = default;

 public:

  ~WarningState() override = default;
  static RadarState* instance();
  void start(RadarContext *c) override;
  void update(RadarContext *c, uint32_t input) override;
};

#endif //A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_
