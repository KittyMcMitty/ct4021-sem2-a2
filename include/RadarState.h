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
const uint8_t trigger_pin PROGMEM = A1, echo_pin = 3,
    servo_pin PROGMEM = 9;

// LCD pins
const uint8_t d4 PROGMEM = 8, d5 PROGMEM = 7, d6 = 4, d7 PROGMEM = 2,
    rs PROGMEM = 13, en PROGMEM = 12;

// IR sensor
const uint8_t ir_pin PROGMEM = 11;

// LED pins
const uint8_t red_pin PROGMEM = 10, green_pin PROGMEM = 6, blue_pin PROGMEM = 5;
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
  LiquidCrystal lcd_ {CFG::rs, CFG::en, CFG::d4, CFG::d5, CFG::d6, CFG::d7};
#endif


  RadarState* state_ {nullptr};

  uint32_t timer_; // track how long since measurement in range


  
  void change_state(RadarState* s);
  void command_add_entry(FunctionObject *func, uint16_t frequency);
  void command_remove_entry(FunctionObject *func);
  void led_set_colour(LEDColour colour);
  void led_set_pulse(int8_t  increment);
  void lcd_setCursor(uint8_t row, uint8_t col);
  void lcd_print(const char str[]);
  uint32_t get_timer() const;
  void set_timer();

 public:
  RadarContext();
  ~RadarContext();

  void radar_move();
  uint32_t radar_ping();
  void led_pulse();

  void start();
  void stop();
  void update(uint32_t);
};


class RadarState {
 protected:
  static void change_state(RadarContext* c, RadarState* s);
  static void radar_move(RadarContext* c);
  static uint32_t radar_ping(RadarContext* c);
  static void command_add_entry(RadarContext *c,
                                FunctionObject *func,
                                uint16_t frequency);
  static void command_remove_entry(RadarContext *c, FunctionObject *func);
  static void led_set_colour(RadarContext* c, LEDColour colour);
  static void led_set_pulse(RadarContext* c, int8_t  increment);
  static void led_pulse(RadarContext* c);
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
