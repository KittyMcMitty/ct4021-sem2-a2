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
// LCD pins
const uint8_t d0 PROGMEM {A2}, d1 PROGMEM {A3}, d2 PROGMEM {A4},
    d3 PROGMEM {A5}, d4 PROGMEM {13}, d5 PROGMEM {12}, d6 PROGMEM {8},
    d7 PROGMEM {7}, rs PROGMEM {A0},  en PROGMEM {A1};

// Radar pins
const uint8_t trigger_pin PROGMEM {4}, echo_pin {2},
    servo_pin PROGMEM {9};

// IR sensor
const uint8_t ir_pin PROGMEM {11};

// LED pins
const uint8_t red_pin PROGMEM {3}, green_pin PROGMEM {6}, blue_pin PROGMEM {5};

const uint8_t buzzer_pin PROGMEM {10};

// distance thresholds for radar
const uint32_t  distance_warning PROGMEM {60}, distance_red PROGMEM {300},
    distance_yellow PROGMEM {600}, distance_green PROGMEM {900};

const uint32_t standby_timeout PROGMEM {10000};

} // namespace CFG


class RadarContext;
class RadarState;
class StandbyState;
class SensingState;
class WarningState;

/*
 * RadarContext
 */
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

  /*
   * Change State
   *
   * RadarState* s - the new state to transition to
   */
  TEST_VIRTUAL void change_state(RadarState* s); //

  /*
   * Command Add Entry
   *
   * Add an entry to the command queue
   *
   * FunctionObject* func - the function object to add
   * uint16_t frequency - how often to call the object in milliseconds
   */
  TEST_VIRTUAL void command_add_entry(FunctionObject *func, uint16_t frequency);

  /*
   * Command Remove Entry
   *
   * FunctionObject* func - function object to remove
   */
  TEST_VIRTUAL void command_remove_entry(FunctionObject *func);

  /*
   * LED Set Colour
   *
   * LEDColour colour - red, yellow or green (from MyLED.h)
   */
  TEST_VIRTUAL void led_set_colour(LEDColour colour);

  /*
   * LED Set Pulse
   *
   * int8_t increment - how much to vary the brightness each time led_pulse()
   *                    is called. Max brightness is 255.
   */
  TEST_VIRTUAL void led_set_pulse(int8_t  increment);

  /*
   * Get Timer
   *
   * RadarContext tracks how long it has been since something is in range. This
   * method returns that time in milliseconds
   *
   * Returns:
   *
   * uint32_t - time value in milliseconds
   */
  TEST_VIRTUAL uint32_t get_timer() const;

  /*
   * Set Timer
   *
   * RadarContext tracks how long it has been since something is in range. This
   * method sets that time in milliseconds. State objects can call this to set
   * the value.
   */
  TEST_VIRTUAL void set_timer();

 public:
  /*
   * Constructor
   *
   * RadarState* state - Initial state. If nothing is passed it defaults to
   *                     standby.
   */
  explicit RadarContext(RadarState* state = nullptr);
  TEST_VIRTUAL ~RadarContext();

  /*
   * Init
   *
   * This method initialises and resets everything back to standby and then
   * starts the standby state.
   */
  TEST_VIRTUAL void init();

  /*
   * Radar Move
   *
   * Move the radar 1°
   */
  TEST_VIRTUAL void radar_move();

  /*
   * Radar Ping
   *
   * LEAVE A 500MS DELAY BETWEEN CALLS TO THIS METHOD!
   *
   * This method will ping the radar and then return the value from the LAST
   * time it was called. This is because the HC-SR04 sensor will return a pulse
   * lasting up to 500ms, so rather than block everything for half a second
   * there is an ISR that sets the pulse start/end times and this method returns
   * a value based on those numbers when they are available.
   *
   * As a consequence, ONLY CALL THIS METHOD EVERY 500MS OR MORE!
   */
  TEST_VIRTUAL uint32_t radar_ping();

  /*
   * LED Pulse
   *
   * modifify the LED brightness by the value configured in set_pulse()
   */
  TEST_VIRTUAL void led_pulse();

  /*
   * LCD Set Cursor
   *
   * Set the LCD cursor to position on screen.
   *
   * setCursor is camelCase to match the Arduino LiquidCrystal library this
   * method wraps.
   *
   * uint8_t col - column
   * uint8_t row - row
   */
  TEST_VIRTUAL void lcd_setCursor(uint8_t col, uint8_t row);

  /*
   * LCD Print
   *
   * Print something to screen
   *
   * const char str[] - a string
   */
  TEST_VIRTUAL void lcd_print(const char str[]);

  // int n - an integer
  TEST_VIRTUAL void lcd_print(int n);

  /*
   * LCD Clear
   *
   * Clear the screen
   */
  TEST_VIRTUAL void lcd_clear();

  /*
   * Execute Current Entry
   *
   * Executes the current command queue entry. The queue is smart and will
   * figure out what it needs to do next.
   *
   * Returns:
   * uint32_t - time in milliseconds at which next command expects to be
   *            executed.
   */
  TEST_VIRTUAL uint32_t execute_current_entry();

  /*
   * Start
   *
   * This method is state dependant. It is passed through to the current state
   */
  TEST_VIRTUAL void start();

  /*
   * Update
   *
   * This method is state dependant. It is passed through to the current state
   *
   * uint32_t value - The value to be passed to the state
   */
  TEST_VIRTUAL void update(uint32_t);
};

/*
 * RadarState Abstract Base Class
 *
 * This class is friends with RadarContext, meaning it can access protected or
 * private methods. RadarState child classes use the methods in this class to
 * access or update the state stored in RadarContext.
 */
class RadarState {
 protected:
  /*
   * All these methods are just wrappers for the same methods in RadarContext.
   */
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

  /*
   * These methods are pure virtual. The concrete states implement them
   */
  virtual void start(RadarContext *c) = 0;
  virtual void update(RadarContext *c, uint32_t input) = 0;
};

/*
 * StandbyState
 *
 * In this state, the system is blinking the LED green and checking the PIR
 * sensor. If anything is detected, it will transition to SensingState.
 *
 * This class is a singleton.
 */
class StandbyState : public RadarState {

 private:
  StandbyState() = default;
  inline static RadarState* instance_ {nullptr}; // instance pointer
  ~StandbyState() final = default;

 public:
  /*
   * Instance
   *
   * Return pointer to singleton instance.
   */
  static RadarState* instance();

  /*
   * Delete Instance
   *
   * Delete the singleton and reset instance to nullptr.
   *
   * This was put here for testing, and is is probably a bad idea to do in
   * release.
   */
  static void delete_instance();

  /*
   * Start
   *
   * This method will add the following commands:
   *
   * DoPIRCheck
   * DoLEDPulse
   *
   * It will set the LED colour to Green.
   *
   * RadarContext* c - pointer to the Radar Context.
   */
  void start(RadarContext *c) final;

  /*
   * Update
   *
   * This method will clear previously registered commands and transition to
   * sensing state on any non 0 input.
   *
   * RadarContext* c - pointer to the Radar Context.
   * uint32_t input - input value
   */
  void update(RadarContext *c, uint32_t input) final;
};

class SensingState : public RadarState {
 private:
  /*
   * I
   */
  inline static RadarState* instance_ {nullptr};
  SensingState() = default;
  ~SensingState() final = default;

  /*
   * Change Standby
   *
   * This method will remove the following commands:
   *
   * DoMove
   * DoPing
   *
   * it will then transition to standby
   *
   * RadarContext* c - pointer to the Radar Context.
   */
  static void change_standby(RadarContext* c);

  /*
   * Change Warning
   *
   * This method will transition to WarningState
   *
   * RadarContext* c - pointer to the Radar Context.
   */
  static void change_warning(RadarContext* c);

 public:
  /*
   * Instance
   *
   * Return pointer to singleton instance
   */
  static RadarState* instance();

  /*
   * Delete Instance
   *
   * Delete the singleton and reset instance to nullptr.
   *
   * This was put here for testing, and is is probably a bad idea to do in
   * release.
   */
  static void delete_instance();

  /*
   * Start
   *
   * This method will register the following commands:
   *
   * DoMove
   * DoPing
   */
  void start(RadarContext *c) final;

  /*
   * Update
   *
   * This method takes a distance and will update the LED colour or transition
   * to a new state depending upon the value
   *
   * RadarContext* c - pointer to the Radar Context.
   * uint32_t input - distance as returned from radar.ping()
   */
  void update(RadarContext *c, uint32_t input) final;
};

class WarningState : public RadarState {
 private:
  inline static RadarState* instance_ {nullptr};
  WarningState() = default;
  ~WarningState() final = default;

 public:
  /*
   * Instance
   *
   * Return pointer to singleton instance
   */
  static RadarState* instance();

  /*
   * Delete Instance
   *
   * Delete the singleton and reset instance to nullptr.
   *
   * This was put here for testing, and is is probably a bad idea to do in
   * release.
   */
  static void delete_instance();

  /*
   * Start
   *
   * This method will register the following commands:
   *
   * DoLEDPulse
   *
   * It will set the LED colour to red
   * It will sound the buzzer
   *
   * RadarContext* c - pointer to the Radar Context.
   */
  void start(RadarContext *c) final;

  /*
   * Update
   *
   * This method takes a distance. If the distance is beyond the warning range,
   * it will transition back to sensing state.
   *
   * RadarContext* c - pointer to the Radar Context.
   * uint32_t input - distance as returned from radar.ping()
   */
  void update(RadarContext *c, uint32_t input) final;
};
#endif //A_TOOLCHAIN_TEST_SRC_RADARSTATE_H_
