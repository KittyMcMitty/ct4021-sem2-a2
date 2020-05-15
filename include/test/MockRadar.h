//
// Created by Nicholas Ives on 13/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKRADAR_H_
#define A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKRADAR_H_

#include <gmock/gmock.h>

class MockRadar {
 public:
  MockRadar();
  ~MockRadar();
  MOCK_METHOD(void, move, ());
  MOCK_METHOD(uint32_t, ping, ());
  MOCK_METHOD(void, init, (uint8_t, uint8_t, uint8_t));
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

#endif //A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKRADAR_H_
