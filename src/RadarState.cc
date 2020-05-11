//
// Created by Nicholas Ives on 08/05/2020.
//

#include <RadarState.h>
#include <CommandQueue.h>
#include <ArduinoInterface.h>
#include <RadarStateMocks.h>

// All our action objects can share this context pointer
class RadarAction : public FunctionObject {
 protected:
  static RadarContext* context_;
 public:
  explicit RadarAction(RadarContext* c) {
    if (!context_) {
      context_ = c;
    }
  }
};

// move radar
class DoMove : public RadarAction {
  public:
  void operator()() override {
    context_->radar_move();
  }
};

// ping the radar and stick it back into the context
class DoPing : public RadarAction {
 public:
  void operator()() override {
    uint32_t distance = context_->radar_ping();
    context_->update(distance);
  }
};

// pulse LED
class DoPulse : public RadarAction {
 public:
  void operator()() override  {
    context_->led_pulse();
  }
};


class DoPIRCheck : RadarAction {
 public:
  void operator()() override {
    using AI = ArduinoInterface;
    uint8_t sensor = AI::digitalRead(CFG::ir_pin);
    if (sensor) {
      context_->update(1);
    }
  }
};

/* * * * * * * * *
 * RadarContext  *
 * * * * * * * * *
 */

// Set initial state to standby
RadarContext::RadarContext() {
  state_ = StandbyState::instance();
}

// During normal operation, all these state objects will be made.
RadarContext::~RadarContext() {
    delete StandbyState::instance();
    delete SensingState::instance();
    delete WarningState::instance();
}

void RadarContext::change_state(RadarState *state) {
  state_ = state;
}

void RadarContext::start() {
  state_->start(this);
}

void RadarContext::stop() {
  state_->stop();
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
void RadarContext::command_add_entry() {

}
void RadarContext::command_remove_entry() {

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
void RadarContext::lcd_setCursor(uint8_t row, uint8_t col) {
  lcd_.setCursor(row, col);
}
void RadarContext::lcd_print(const char *str) {
  lcd_.print(str);
}

uint32_t RadarContext::get_timer() const {
  return timer_;
}
void RadarContext::set_timer() {
  timer_ = ArduinoInterface::millis();
}

/* * * * * * * *
 * RadarState  *
 * * * * * * * *
 */

void RadarState::change_state(RadarContext *c, RadarState *state) {
  c->change_state(state);
}
void RadarState::radar_move(RadarContext *c) {
  c->radar_move();
}
uint32_t RadarState::radar_ping(RadarContext *c) {
  return c->radar_ping();
}
void RadarState::command_add_entry(RadarContext *c) {
  c->command_add_entry();
}
void RadarState::command_remove_entry(RadarContext *c) {
  c->command_remove_entry();
}
void RadarState::led_set_colour(RadarContext *c, LEDColour colour) {
  c->led_set_colour(colour);
}
void RadarState::led_set_pulse(RadarContext *c, int8_t increment) {
  c->led_set_pulse(increment);
}
void RadarState::led_pulse(RadarContext *c) {
  c->led_pulse();
}
void RadarState::lcd_setCursor(RadarContext *c, uint8_t row, uint8_t col) {
  c->lcd_setCursor(row,col);
}
void RadarState::lcd_print(RadarContext *c, const char *str) {
  c->lcd_print(str);
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

// instance methods. Returns single instance.
RadarState *StandbyState::instance() {
  if (instance_ == nullptr) {
    instance_ = new StandbyState;
  }
  return instance_;
}


void StandbyState::start(RadarContext *c) {

}


void StandbyState::stop() {

}


void StandbyState::update(RadarContext *c, uint32_t input) {
  if (input) {
    change_state(c, SensingState::instance());
  }
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

}
void SensingState::stop() {

}
void SensingState::update(RadarContext *c, uint32_t distance) {

  if (distance < 1000) {
    set_timer(c);
    change_state(c, WarningState::instance());
  } else if (distance < 1750) {
    set_timer(c);
    led_set_colour(c, LEDColour::RED);
  } else if (distance < 2500) {
    set_timer(c);
    led_set_colour(c, LEDColour::YELLOW);
  } else {
    led_set_colour(c, LEDColour::GREEN);

    uint32_t time = ArduinoInterface::millis();
    // if more than 30s have passed
    if (time - get_timer(c) > 30000) {
      change_state(c, StandbyState::instance());
    }
  }
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

}
void WarningState::stop() {

}

void WarningState::update(RadarContext *c, uint32_t distance) {
  if (distance > 800) {
    change_state(c, SensingState::instance());
  }
}
