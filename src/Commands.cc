//
// Created by Nicholas Ives on 13/05/2020.
//

#include <Commands.h>
#include <new.h>

RadarAction::RadarAction() = default;

RadarAction *DoMove::instance(RadarContext *c) {
  if (instance_ == nullptr) {
    instance_ = new DoMove;
  }
  context_ = c;
  return instance_;
}

void DoMove::operator()() {
  context_->radar_move();
}
void DoMove::delete_instance() {
  delete instance_;
  instance_ = nullptr;
}

RadarAction *DoPing::instance(RadarContext *c) {
  if (instance_ == nullptr) {
    instance_ = new DoPing;
  }
  context_ = c;
  return instance_;
}

void DoPing::operator()() {
  uint32_t distance = context_->radar_ping();
  context_->lcd_setCursor(0, 1);
  context_->lcd_print("Distance: ");
  if (distance != UINT32_MAX) {
    context_->lcd_print(distance);
    context_->lcd_print("  ");
  }  else {
    context_->lcd_print("***");
  }

  context_->update(distance);
}
void DoPing::delete_instance() {
  delete instance_;
  instance_ = nullptr;
}

RadarAction *DoLEDPulse::instance(RadarContext *c) {
  if (instance_== nullptr) {
    instance_ = new DoLEDPulse;
  }
  context_ = c;
  return instance_;
}

void DoLEDPulse::operator()() {
  context_->led_pulse();
}
void DoLEDPulse::delete_instance() {
  delete instance_;
  instance_ = nullptr;
}

RadarAction *DoPIRCheck::instance(RadarContext *c) {
  if (instance_== nullptr) {
    instance_ = new DoPIRCheck;
  }
  context_ = c;
  return instance_;
}

void DoPIRCheck::operator()() {
  using AI = ArduinoInterface;
  uint8_t sensor = AI::digitalRead(CFG::ir_pin);
  count += 1;
  context_->lcd_setCursor(0,1);
  context_->lcd_print("Count: ");
  context_->lcd_print(count);

  if (sensor) {
    count = 0;
    context_->update(1);
  }
}

void DoPIRCheck::delete_instance() {
  RadarAction* tmp = instance_;
  delete tmp;
  instance_ = nullptr;
}
/*
RadarAction *DoMemStats::instance(RadarContext *c) {
  if (instance_== nullptr) {
    instance_ = new DoMemStats;
  }
  context_ = c;
  return instance_;
}

void DoMemStats::delete_instance() {
  RadarAction* tmp = instance_;
  delete tmp;
  instance_ = nullptr;
}

void DoMemStats::operator()() {
  context_->lcd_setCursor(0,0);
  context_->lcd_print("A:");
  context_->lcd_print(current_alloc);
  context_->lcd_print("M:");
  context_->lcd_print(max_alloc);
}
*/