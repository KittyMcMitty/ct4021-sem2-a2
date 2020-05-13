//
// Created by Nicholas Ives on 11/05/2020.
//

#include <gmock/gmock.h>
#include <RadarStateMocks.h>
#include <RadarState.h>
#include <memory>

class MockRadarContext {
 public:
  MOCK_METHOD(void, change_state, (RadarState*));
  MOCK_METHOD(void, command_add_entry, (FunctionObject*, uint16_t));
  MOCK_METHOD(void, command_remove_entry, (FunctionObject*));
  MOCK_METHOD(void, led_set_colour, (LEDColour));
  MOCK_METHOD(void, led_set_pulse, (int8_t));
  MOCK_METHOD(uint32_t, get_timer, ());
  MOCK_METHOD(void, set_timer, ());
  MOCK_METHOD(void, start, ());
  MOCK_METHOD(void, init, ());
  MOCK_METHOD(void, radar_move, ());
  MOCK_METHOD(uint32_t, radar_ping, ());
  MOCK_METHOD(void, led_pulse, ());
  MOCK_METHOD(void, lcd_setCursor, (uint8_t, uint8_t));
  MOCK_METHOD(void, lcd_print, (const char *));
  MOCK_METHOD(void, lcd_print, (int n));
  MOCK_METHOD(uint32_t, execute_current_entry, ());
  MOCK_METHOD(void, update, (uint32_t));
};

class MockRadarState : public RadarState {
  MOCK_METHOD(void, start, (RadarContext*), (override));
  MOCK_METHOD(void, update, (RadarContext*, uint32_t), (override));
};


 class RadarContextTest : public ::testing::Test {
  protected:
  MockMyLED mock_my_led_;
  MockCommandQueue mock_command_queue_;
  MockRadar mock_radar_;
  MockLiquidCrystal mock_liquid_crystal_;

  RadarContextTest() {
    MyLEDMockInterface::mock_led_ = &mock_my_led_;
    CommandQueueMockInterface::mock_queue_ = &mock_command_queue_;
    RadarMockInterface::mock_radar_ = &mock_radar_;
    LiquidCrystalMockInterface::mock_lcd_ = &mock_liquid_crystal_;
  }
};

 class StandbyStateTest : public ::testing::Test {
  protected:
   MockRadarContext mock_radar_context_;
   RadarContext* radar_context_;
   RadarState* standby_state_;

   StandbyStateTest() {
     standby_state_ = StandbyState::instance();
   }

   ~StandbyStateTest() {
     delete standby_state_;
   }
 };

 class test_class {
  public:
   int* iptr;
 };

 TEST_F(StandbyStateTest, TestUpdate) {
   //radar_context_ = mock_radar_context_;
   //standby_state_->update(&mock_radar_context_, 1);


 }