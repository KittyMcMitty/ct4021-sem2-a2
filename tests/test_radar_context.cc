//
// Created by Nicholas Ives on 11/05/2020.
//

#include <gmock/gmock.h>
#include <RadarStateMocks.h>
#include <RadarState.h>
#include <memory>



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
   //radar_context_ = static_cast<MockRadarContext*>(&mock_radar_context_);
   //standby_state_->update(&mock_radar_context_, 1);


 }