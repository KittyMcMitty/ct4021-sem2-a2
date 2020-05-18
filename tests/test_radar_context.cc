//
// Created by Nicholas Ives on 11/05/2020.
//

#include <gmock/gmock.h>
#include <ArduinoInterface.h>
#include <test/MockCommandQueue.h>
#include <test/MockMyLED.h>
#include <test/MockLiquidCrystal.h>
#include <test/MockRadar.h>
#include <Commands.h>
#include <RadarState.h>
#include <test/MockRadarContext.h>

// all these fixtures need to be initialised before our MockRadarContext can be
// constructed (we SIG11 otherwise) so do this here.
class RadarContextMockDependancies : public ::testing::Test {
 protected:

  testing::NiceMock<MockMyLED> mock_my_led_;
  testing::NiceMock<MockCommandQueue> mock_command_queue_;
  testing::NiceMock<MockRadar> mock_radar_;
  testing::NiceMock<MockLiquidCrystal> mock_liquid_crystal_;
  MockArduinoClass mock_arduino_interface_;

  // The concrete states use these distance measures.
  uint32_t  distance_warning {CFG::distance_warning-1},
            distance_red {CFG::distance_red-1},
            distance_yellow {CFG::distance_yellow-1},
            distance_green {CFG::distance_green};

  RadarContextMockDependancies() {
    MyLEDMockInterface::mock_led_ = &mock_my_led_;
    CommandQueueMockInterface::mock_queue_ = &mock_command_queue_;
    RadarMockInterface::mock_radar_ = &mock_radar_;
    LiquidCrystalMockInterface::mock_lcd_ = &mock_liquid_crystal_;
    MockArduino::mock = &mock_arduino_interface_;
  }
};


/*
 * RadarContextTest fixtures
 */

class RadarContextTest : public RadarContextMockDependancies {
 protected:

  MockRadarState mock_radar_state_; // mock state object
  RadarContext radar_context_ {&mock_radar_state_}; // class under test

  RadarContextTest() = default;
  ~RadarContextTest() override = default;


  // Test class is friend of actual class to test private methods
  void change_state(RadarState* s) {
    radar_context_.change_state(s);
  }

  void command_add_entry(FunctionObject *func, uint16_t frequency) {
    radar_context_.command_add_entry(func, frequency);
  };
  void command_remove_entry(FunctionObject *func) {
    radar_context_.command_remove_entry(func);
  };
  void led_set_colour(LEDColour colour) {
    radar_context_.led_set_colour(colour);
  };
  void led_set_pulse(int8_t  increment) {
    radar_context_.led_set_pulse(increment);
  };
  uint32_t get_timer() const {
    return radar_context_.get_timer();
  };
  void set_timer() {
    radar_context_.set_timer();
  };

};

/*
 * This is used in the command queue tests
 */
class TestFuncObj : public FunctionObject {

 public:
  void operator()() final {}
};

/*
 * RadarContext tests.
 */

TEST_F(RadarContextTest, TestInit) {
  using testing::_;
  using testing::AnyNumber;
  using namespace CFG;

  EXPECT_CALL(mock_command_queue_, clear_queue())
      .Times(1);

  EXPECT_CALL(mock_command_queue_, add_entry(_,_))
      .Times(AnyNumber());

  EXPECT_CALL(mock_arduino_interface_,pinMode(ir_pin, INPUT))
      .Times(1);

  EXPECT_CALL(mock_radar_, init(trigger_pin, echo_pin, servo_pin))
      .Times(1);

  EXPECT_CALL(mock_liquid_crystal_, begin(16,2))
      .Times(1);

  EXPECT_CALL(mock_my_led_, set_colour(_));
  EXPECT_CALL(mock_my_led_, set_pulse(_));

  radar_context_.init();

  DoLEDPulse::delete_instance();
}

TEST_F(RadarContextTest, TestChangeState) {
  MockRadarState second_mock_state;

  EXPECT_CALL(second_mock_state, start(&radar_context_))
      .Times(1);

  change_state(&second_mock_state);
  radar_context_.start();
}

TEST_F(RadarContextTest, TestStart) {
  EXPECT_CALL(mock_radar_state_, start(&radar_context_))
      .Times(1);
  radar_context_.start();
}

TEST_F(RadarContextTest, TestUpdate) {
  EXPECT_CALL(mock_radar_state_, update(&radar_context_, 1))
      .Times(1);

  radar_context_.update(1);
}

TEST_F(RadarContextTest, TestRadarMove) {
  EXPECT_CALL(mock_radar_, move())
      .Times(1);

  radar_context_.radar_move();
}

TEST_F(RadarContextTest, TestRadarPing) {
  using testing::Return;

  uint32_t r_value {400};

  EXPECT_CALL(mock_radar_, ping())
      .Times(1)
      .WillRepeatedly(Return(r_value));

  uint32_t result = radar_context_.radar_ping();

  ASSERT_EQ(result, r_value);
}

TEST_F(RadarContextTest, TestCommandAddExecuteRemoveEntry) {
  TestFuncObj test_fun;
  uint16_t frequency {100};

  EXPECT_CALL(mock_command_queue_, add_entry(&test_fun, frequency))
      .Times(1);

  EXPECT_CALL(mock_command_queue_, execute_current_entry())
      .Times(1);

  EXPECT_CALL(mock_command_queue_, remove_entry(&test_fun))
      .Times(1);

  command_add_entry(&test_fun, frequency);
  radar_context_.execute_current_entry();
  command_remove_entry(&test_fun);
}


TEST_F(RadarContextTest, TestLedSetColour) {
  EXPECT_CALL(mock_my_led_, set_colour(LEDColour::YELLOW))
      .Times(1);

  led_set_colour(LEDColour::YELLOW);
}

TEST_F(RadarContextTest, TestLedSetPulse) {
  EXPECT_CALL(mock_my_led_, set_pulse(5))
      .Times(1);

  led_set_pulse(5);
}

TEST_F(RadarContextTest, TestSetGetTimer) {
  using testing::Return;

  uint32_t result {0}, input {100};

  EXPECT_CALL(mock_arduino_interface_, millis())
      .Times(1)
      .WillRepeatedly(Return(input));

  set_timer();
  result = get_timer();

  ASSERT_EQ(input, result);
}

TEST_F(RadarContextTest, TestLedPulse) {
  EXPECT_CALL(mock_my_led_, pulse())
      .Times(1);

  radar_context_.led_pulse();
}

TEST_F(RadarContextTest, TestLCDSetCursor) {
  EXPECT_CALL(mock_liquid_crystal_, setCursor(16,2))
      .Times(1);

  radar_context_.lcd_setCursor(16,2);
}

TEST_F(RadarContextTest, TestLCDPrint) {
  const char test_str[5] {"test"};
  const int test_int {10};

  EXPECT_CALL(mock_liquid_crystal_, print(test_int))
      .Times(1);
  EXPECT_CALL(mock_liquid_crystal_, print(test_str))
      .Times(1);

  radar_context_.lcd_print(test_int);
  radar_context_.lcd_print(test_str);
}

/*
 * StandbyState tests
 */

class StandbyStateTest : public RadarContextMockDependancies {
 protected:

  RadarState* standby_state_;
  MockRadarContext mock_radar_context_;
  RadarAction* pir_command_;
  RadarAction* led_command_;

  StandbyStateTest() {
   standby_state_ = StandbyState::instance();

    pir_command_ = DoPIRCheck::instance(&mock_radar_context_);
    led_command_ = DoLEDPulse::instance(&mock_radar_context_);
  }
  ~StandbyStateTest() {
    StandbyState::delete_instance();

    DoPIRCheck::delete_instance();
    DoLEDPulse::delete_instance();
  }
};

TEST_F(StandbyStateTest, TestStart) {
  using ::testing::_;
  using testing::Matcher;

  // Is the LED set to green?
  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::GREEN))
      .Times(1);

  // Is a pulse set? (value doesn't really matter)
  EXPECT_CALL(mock_radar_context_, led_set_pulse(_))
      .Times(1);

  // Is the PIR sensor check function added? Frequency not important
  EXPECT_CALL(mock_radar_context_, command_add_entry(
      pir_command_, _))
      .Times(1);

  // Is the DoPulse function added? Frequency not important
  EXPECT_CALL(mock_radar_context_, command_add_entry(
      led_command_, _))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, lcd_print(Matcher<const char *>(_)))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, lcd_setCursor(0,0))
      .Times(1);

  standby_state_->start(&mock_radar_context_);

}

TEST_F(StandbyStateTest, TestUpdate) {
  RadarState* sensing_state = SensingState::instance();

  EXPECT_CALL(mock_radar_context_, command_remove_entry(
      pir_command_))
      .Times(1);
  EXPECT_CALL(mock_radar_context_, command_remove_entry(
      led_command_))
      .Times(1);
  EXPECT_CALL(mock_radar_context_, change_state(sensing_state))
      .Times(1);
  EXPECT_CALL(mock_radar_context_, start())
      .Times(1);

  standby_state_->update(&mock_radar_context_, 1);

}

/*
 * SensingState tests
 */

class SensingStateTest : public RadarContextMockDependancies {
 protected:

  RadarState* sensing_state_;
  MockRadarContext mock_radar_context_;
  RadarAction* move_command_;
  RadarAction* ping_command_;

  SensingStateTest() {
    sensing_state_ = SensingState::instance();
    move_command_ = DoMove::instance(&mock_radar_context_);
    ping_command_ = DoPing::instance(&mock_radar_context_);
  }

  ~SensingStateTest() {
    SensingState::delete_instance();
    DoMove::delete_instance();
    DoPing::delete_instance();
  }
};

TEST_F(SensingStateTest, TestStart) {
  using testing::_;
  using testing::Ge;

  // Does DoMove get added?
  EXPECT_CALL(mock_radar_context_, command_add_entry(
      move_command_, _))
      .Times(1);

  // Does ping get added? Is the frequency >= 500?
  EXPECT_CALL(mock_radar_context_, command_add_entry(
      ping_command_, Ge(500)))
      .Times(1);

  sensing_state_->start(&mock_radar_context_);
}

TEST_F(SensingStateTest, TestUpdateRedWarning) {

  auto warning_state = WarningState::instance();

  EXPECT_CALL(mock_radar_context_, set_timer())
      .Times(1);

  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::RED))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, change_state(warning_state))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, start())
      .Times(1);

  sensing_state_->update(&mock_radar_context_, distance_warning);

  WarningState::delete_instance();
}

TEST_F(SensingStateTest, TestUpdateRed) {
  EXPECT_CALL(mock_radar_context_, set_timer())
      .Times(1);

  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::RED))
      .Times(1);

  sensing_state_->update(&mock_radar_context_, distance_red);
}

TEST_F(SensingStateTest, TestUpdateYellow) {
  EXPECT_CALL(mock_radar_context_, set_timer())
      .Times(1);

  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::YELLOW))
      .Times(1);

  sensing_state_->update(&mock_radar_context_, distance_yellow);
}

TEST_F(SensingStateTest, TestUpdateGreen) {
  using testing::Return;
  using testing::_;

  // On this path, millis is called to get current time
  // if we return 0, we should not enter standby
  EXPECT_CALL(mock_arduino_interface_, millis())
      .Times(1)
      .WillRepeatedly(Return(0));

  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::GREEN))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, get_timer())
      .Times(1);

  // make sure change_state is NOT called
  EXPECT_CALL(mock_radar_context_, change_state(_))
      .Times(0);

  sensing_state_->update(&mock_radar_context_, distance_green);
}

TEST_F(SensingStateTest, TestUpdateGreenStandby) {
  using testing::Return;
  using testing::_;

  // On this path, millis is called to get current time
  // if we return 30,000, we should enter standby
  EXPECT_CALL(mock_arduino_interface_, millis())
      .Times(1)
      .WillRepeatedly(Return(30000));

  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::GREEN))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, get_timer())
      .Times(1);

  EXPECT_CALL(mock_radar_context_, command_remove_entry(
          move_command_))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, command_remove_entry(
          ping_command_))
      .Times(1);

  // make sure change_state is called
  EXPECT_CALL(mock_radar_context_, change_state(_))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, start())
      .Times(1);

  sensing_state_->update(&mock_radar_context_, distance_green);
}

/*
 * WarningState tests
 */

class WarningStateTest : public RadarContextMockDependancies {
 protected:

  RadarState* warning_state_;
  MockRadarContext mock_radar_context_;
  RadarAction* led_command_;

  WarningStateTest() {
    warning_state_ = WarningState::instance();
    led_command_ = DoLEDPulse::instance(&mock_radar_context_);
  }

  ~WarningStateTest() {
    WarningState::delete_instance();
    DoLEDPulse::delete_instance();
  }
};

TEST_F(WarningStateTest, TestStart) {
  using testing::_;

  EXPECT_CALL(mock_radar_context_, led_set_pulse(_))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, led_set_colour(LEDColour::RED))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, command_add_entry(
          led_command_, _))
      .Times(1);

  EXPECT_CALL(mock_arduino_interface_, tone(CFG::buzzer_pin, _, _))
      .Times(1);

  warning_state_->start(&mock_radar_context_);

}

TEST_F(WarningStateTest, TestUpdate) {
  using testing::_;

  // make sure change_state is NOT called
  EXPECT_CALL(mock_radar_context_, change_state(_))
      .Times(0);

  warning_state_->update(&mock_radar_context_, distance_warning);
}

TEST_F(WarningStateTest, TestUpdateSensing) {

  // make sure change_state is NOT called
  EXPECT_CALL(mock_radar_context_, change_state(SensingState::instance()))
      .Times(1);

  EXPECT_CALL(mock_radar_context_, command_remove_entry(
          led_command_))
      .Times(1);

  EXPECT_CALL(mock_arduino_interface_, noTone(CFG::buzzer_pin))
      .Times(1);

  warning_state_->update(&mock_radar_context_, distance_red);

}