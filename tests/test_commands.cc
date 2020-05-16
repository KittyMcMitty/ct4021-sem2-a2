//
// Created by Nicholas Ives on 14/05/2020.
//

#include <gmock/gmock.h>
#include <Commands.h>
#include <test/MockRadarContext.h>

 class CommandsTest : public ::testing::Test {
  protected:
  MockRadarContext mock_radar_context_;

  CommandsTest() {
    RadarAction::context_ = &mock_radar_context_;
  }
  ~CommandsTest() {
    RadarAction::context_ = nullptr;
  }
};

TEST_F(CommandsTest, DoMoveTest) {

  auto command = DoMove::instance(&mock_radar_context_);

  EXPECT_CALL(mock_radar_context_, radar_move())
      .Times(1);

  (*command)();
}

TEST_F(CommandsTest, DoPingTest) {
  using testing::Return;
  using testing::_;
  using testing::Matcher;

  uint32_t r_value {300};

  auto command = DoPing::instance(&mock_radar_context_);


  EXPECT_CALL(mock_radar_context_, radar_ping())
      .Times(1)
      .WillRepeatedly(Return(r_value));

  EXPECT_CALL(mock_radar_context_, lcd_setCursor(_,_))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, lcd_print(Matcher<const char *>(_)))
      .Times(2);

  EXPECT_CALL(mock_radar_context_, lcd_print(r_value))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, update(r_value))
      .Times(1);


  (*command)();
}

// check PIR sensor returns LOW and state does not change
TEST_F(CommandsTest, DoPIRCheckNull) {
  using testing::_;
  using testing::Return;

  // need mock arduino for this test
  MockArduinoClass mock_arduino_class;
  MockArduino::mock = &mock_arduino_class;

  auto command = DoPIRCheck::instance(&mock_radar_context_);

  // make sure update is NOT called
  EXPECT_CALL(mock_radar_context_, update(_))
      .Times(0);

  EXPECT_CALL(mock_arduino_class, digitalRead(_))
      .Times(1)
      .WillRepeatedly(Return(LOW));

  (*command)();
}

// Check PIR sensor returns HIGH and state changes
TEST_F(CommandsTest, DoPIRCheck) {
  using testing::_;
  using testing::Return;

  // need mock arduino for this test
  MockArduinoClass mock_arduino_class;
  MockArduino::mock = &mock_arduino_class;

  auto command = DoPIRCheck::instance(&mock_radar_context_);

  // make sure update is NOT called
  EXPECT_CALL(mock_radar_context_, update(_))
      .Times(1);

  EXPECT_CALL(mock_arduino_class, digitalRead(_))
      .Times(1)
      .WillRepeatedly(Return(HIGH));

  (*command)();
}

/*
TEST_F(CommandsTest, DoLEDPulseTest) {

  auto command = DoLEDPulse::instance(&mock_radar_context_);

  EXPECT_CALL(mock_radar_context_, led_pulse())
      .Times(1);

  (*command)();
}
*/