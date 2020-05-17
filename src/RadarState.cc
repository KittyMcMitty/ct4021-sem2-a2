//
// Created by Nicholas Ives on 08/05/2020.
//

#include <RadarState.h>
#include <ArduinoInterface.h>
#include <Commands.h>

/* * * * * * * * *
 * RadarContext  *
 * * * * * * * * *
 */

// Set initial state to standby
RadarContext::RadarContext(RadarState* state) : led_{CFG::red_pin, CFG::green_pin,
                                    CFG::blue_pin},
                               lcd_{CFG::rs, CFG::en, CFG::d0, CFG::d1,
                                    CFG::d2, CFG::d3, CFG::d4, CFG::d5,
                                    CFG::d6, CFG::d7},
                               state_{state} {
  if (state_ == nullptr) {
    state_ = StandbyState::instance();
  }
};

RadarContext::~RadarContext() = default;

void RadarContext::change_state(RadarState *state) {
  state_ = state;
}

void RadarContext::start() {
  state_->start(this);
}

void RadarContext::update(uint32_t distance) {

  state_->update(this, distance);
}
void RadarContext::radar_move() {
  radar_.move();

}
uint32_t RadarContext::radar_ping() {
  return radar_.ping();

}
void RadarContext::command_add_entry(FunctionObject *func, uint16_t frequency) {
  queue_.add_entry(func, frequency);
}

void RadarContext::command_remove_entry(FunctionObject *func) {
  queue_.remove_entry(func);
}
void RadarContext::led_set_colour(LEDColour colour) {
  led_.set_colour(colour);
}
void RadarContext::led_set_pulse(int8_t increment) {
  led_.set_pulse(increment);
}
void RadarContext::led_pulse() {
  led_.pulse();
}
void RadarContext::lcd_setCursor(uint8_t col, uint8_t row) {
  lcd_.setCursor(col, row);
}
void RadarContext::lcd_print(const char *str) {
  lcd_.print(str);
}

void RadarContext::lcd_print(int n) {
  lcd_.print(n);
}
uint32_t RadarContext::get_timer() const {
  return timer_;
}
void RadarContext::set_timer() {
  timer_ = ArduinoInterface::millis();
}
uint32_t RadarContext::execute_current_entry() {
  return queue_.execute_current_entry();
}
void RadarContext::init() {
  ArduinoInterface::pinMode(CFG::ir_pin, INPUT);
  radar_.init(CFG::trigger_pin, CFG::echo_pin, CFG::servo_pin);
  lcd_.begin(16,2);
  queue_.clear_queue();
  auto state = StandbyState::instance();
  change_state(state);
  start();
}
void RadarContext::lcd_clear() {
  lcd_.clear();
}

/* * * * * * * *
 * RadarState  *
 * * * * * * * *
 * * * * * * * *
 */

void RadarState::change_state(RadarContext *c, RadarState *state) {
  c->change_state(state);
}
void RadarState::command_add_entry(RadarContext *c,
                                   FunctionObject *func,
                                   uint16_t frequency) {
  c->command_add_entry(func, frequency);
}
void RadarState::command_remove_entry(RadarContext *c, FunctionObject *func) {
  c->command_remove_entry(func);
}
void RadarState::led_set_colour(RadarContext *c, LEDColour colour) {
  c->led_set_colour(colour);
}
void RadarState::led_set_pulse(RadarContext *c, int8_t increment) {
  c->led_set_pulse(increment);
}
void RadarState::set_timer(RadarContext *c) {
  c->set_timer();
}
uint32_t RadarState::get_timer(RadarContext *c) {
  return c->get_timer();
}

/* * * * * * * * *
 * StandbyState  *
 * * * * * * * * *
 */

// instance method. Returns single instance.
RadarState *StandbyState::instance() {
  if (instance_ == nullptr) {
    instance_ = new StandbyState;
  }
  return instance_;
}


void StandbyState::start(RadarContext *c) {
  led_set_colour(c, LEDColour::GREEN);
  led_set_pulse(c, 5);
  c->lcd_clear();
  c->lcd_setCursor(0,0);
  c->lcd_print("Standby");

  auto command = DoPIRCheck::instance(c);
  command_add_entry(c, command, 250);

  command = DoLEDPulse::instance(c);
  command_add_entry(c, command, 33);
}

void StandbyState::update(RadarContext *c, uint32_t input) {
  if (input) {
    auto command = DoPIRCheck::instance(c);
    command_remove_entry(c, command);

    command = DoLEDPulse::instance(c);
    command_remove_entry(c, command);

    auto state = SensingState::instance();
    c->lcd_clear();
    change_state(c, state);
    c->start();
  }
}
void StandbyState::delete_instance() {
  delete instance_;
  instance_ = nullptr;
}

/* * * * * * * * *
 * SensingState  *
 * * * * * * * * *
 */
RadarState *SensingState::instance() {
  if (instance_ == nullptr) {
    instance_ = new SensingState;
  }
  return instance_;
}

void SensingState::start(RadarContext *c) {
  auto command = DoMove::instance(c);
  command_add_entry(c, command, 25);

  command = DoPing::instance(c);
  command_add_entry(c, command, 550);

}
void SensingState::update(RadarContext *c, uint32_t distance) {
  using namespace CFG;

  if (distance < distance_warning) {
    set_timer(c);
    led_set_colour(c, LEDColour::RED);
    change_warning(c);
  } else if (distance < distance_red) {
    set_timer(c);
    led_set_colour(c, LEDColour::RED);
  } else if (distance < distance_yellow) {
    set_timer(c);
    led_set_colour(c, LEDColour::YELLOW);
  } else {
    led_set_colour(c, LEDColour::GREEN);

    uint32_t time = ArduinoInterface::millis();
    uint32_t last_time = get_timer(c);
    // if more than 30s have passed
    if (time - last_time >= standby_timeout) {
      change_standby(c);
    }
  }
}

void SensingState::change_standby(RadarContext *c) {
  auto command = DoMove::instance(c);
  command_remove_entry(c, command);

  command = DoPing::instance(c);
  command_remove_entry(c, command);

  auto state = StandbyState::instance();
  change_state(c, state);
  c->start();
}
void SensingState::change_warning(RadarContext *c) {
  auto state = WarningState::instance();
  change_state(c, state);
  c->start();
}
void SensingState::delete_instance() {
  delete instance_;
  instance_ = nullptr;
}

/* * * * * * * * *
 * WarningState  *
 * * * * * * * * *
 */
RadarState *WarningState::instance() {
  if (instance_ == nullptr) {
    instance_ = new WarningState;
  }
  return instance_;
}

void WarningState::start(RadarContext *c) {
  led_set_pulse(c, 12);
  led_set_colour(c, LEDColour::RED);

  auto command = DoLEDPulse::instance(c);
  command_add_entry(c, command, 10);
  ArduinoInterface::tone(CFG::buzzer_pin, 500);
}

void WarningState::update(RadarContext *c, uint32_t distance) {
  if (distance >= CFG::distance_warning) {
    ArduinoInterface::noTone(CFG::buzzer_pin);

    auto command = DoLEDPulse::instance(c);
    command_remove_entry(c, command);

    auto state = SensingState::instance();
    change_state(c, state);
  }
}
void WarningState::delete_instance() {
  delete instance_;
  instance_ = nullptr;
}
