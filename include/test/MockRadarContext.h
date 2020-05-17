//
// Created by Nicholas Ives on 14/05/2020.
//

#ifndef A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKRADARCONTEXT_H_
#define A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKRADARCONTEXT_H_

#include <gmock/gmock.h>
#include <RadarState.h>

class MockRadarContext  : public RadarContext {
 public:
  MockRadarContext();
  virtual ~MockRadarContext();
  MOCK_METHOD(void, change_state, (RadarState*),(override));
  MOCK_METHOD(void, command_add_entry, (FunctionObject*, uint16_t),(override));
  MOCK_METHOD(void, command_remove_entry, (FunctionObject*),(override));
  MOCK_METHOD(void, led_set_colour, (LEDColour),(override));
  MOCK_METHOD(void, led_set_pulse, (int8_t),(override));
  MOCK_METHOD(uint32_t, get_timer, (),(override, const));
  MOCK_METHOD(void, set_timer, (),(override));
  MOCK_METHOD(void, start, (),(override));
  MOCK_METHOD(void, radar_move, (),(override));
  MOCK_METHOD(uint32_t, radar_ping, (),(override));
  MOCK_METHOD(void, led_pulse, (),(override));
  MOCK_METHOD(void, lcd_setCursor, (uint8_t, uint8_t),(override));
  MOCK_METHOD(void, lcd_print, (const char *),(override));
  MOCK_METHOD(void, lcd_print, (int n),(override));
  MOCK_METHOD(void, update, (uint32_t),(override));
};

class MockRadarState : public RadarState {
 public:
  MockRadarState();
  virtual ~MockRadarState();
  MOCK_METHOD(void, start, (RadarContext*), (override));
  MOCK_METHOD(void, update, (RadarContext*, uint32_t), (override));
};

#endif //A_TOOLCHAIN_TEST_INCLUDE_TEST_MOCKRADARCONTEXT_H_
