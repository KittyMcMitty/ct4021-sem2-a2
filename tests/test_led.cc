//
// Created by Nicholas Ives on 29/03/2020.
//

#include <gmock/gmock.h>
#include <MyLED.h>
#include <ArduinoInterface.h>

/*
 * MockArduinoSetup - Set up mock Arduino interface
 *
 * MyLED<MockArduino> needs a mock Arudino interface, but making sure that mock
 * is attached to MockArduino::mock before it is instantiated is hard. So, it is
 * split into two classes.
 *
 * This constructor will be called before any subsequent constructors, so we can
 * sort out MockArduino here.
 */
 class MockArduinoSetup : public ::testing::Test {
 protected:

  // Mock fixtures
  testing::NiceMock<MockArduinoClass>    mock_arduino_class_;

  MockArduinoSetup() {
    MockArduino::mock = &mock_arduino_class_;
  }
};

 /*
  * MyLEDTest - Test MyLED class
  *
  * This class inherits from MockArduinoSetup to ensure that the mock Arduino
  * interface has been set up properly.
  */
 class MyLEDTest : public  MockArduinoSetup {
 protected:

  // LED pins
  const static  uint8_t red_pin_    {1};
  const static  uint8_t green_pin_  {2};
  const static  uint8_t blue_pin_   {3};

  // Mock fixture
  MyLED my_led_ {red_pin_, green_pin_, blue_pin_};

};


TEST_F(MyLEDTest, ConstructorTest) {

  // Do all of our output pins get set correct?
  EXPECT_CALL(mock_arduino_class_, pinMode(red_pin_, OUTPUT))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, pinMode(green_pin_, OUTPUT))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, pinMode(blue_pin_, OUTPUT))
      .Times(1);

  MyLED test_led(red_pin_, green_pin_, blue_pin_);
};


TEST_F(MyLEDTest, SetColourTestRed) {

  EXPECT_CALL(mock_arduino_class_, digitalWrite(red_pin_, HIGH))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, digitalWrite(green_pin_, LOW))
      .Times(1);

  my_led_.set_colour(LEDColour::RED);
}

TEST_F(MyLEDTest, SetColourTestYellow) {

  EXPECT_CALL(mock_arduino_class_, digitalWrite(red_pin_, HIGH))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, digitalWrite(green_pin_, HIGH))
      .Times(1);

  my_led_.set_colour(LEDColour::YELLOW);
}

// Green is the default state, so we have to change to another colour to test it
TEST_F(MyLEDTest, SetColourTestRedThenGreen) {

  EXPECT_CALL(mock_arduino_class_, digitalWrite(red_pin_, HIGH))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, digitalWrite(green_pin_, LOW))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, digitalWrite(red_pin_, LOW))
      .Times(1);

  EXPECT_CALL(mock_arduino_class_, digitalWrite(green_pin_, HIGH))
      .Times(1);

  my_led_.set_colour(LEDColour::RED);
  my_led_.set_colour(LEDColour::GREEN);
}

TEST_F(MyLEDTest, PulseTestGreen) {

  using testing::Sequence;
  using testing::_;
  using testing::AnyNumber;

  Sequence s1, s2;

  int8_t increment = 7; // odd number to really test method
  uint8_t max_brightness = UINT8_MAX;
  uint8_t iterations = 90;

  uint8_t  remainder = max_brightness % increment;


  // do we get calls?
  EXPECT_CALL(mock_arduino_class_, analogueWrite(_, _))
      .Times(AnyNumber());

  // Test top edge

  EXPECT_CALL(mock_arduino_class_,analogueWrite(green_pin_, max_brightness - remainder - increment))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
      analogueWrite(green_pin_, max_brightness - remainder))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
      analogueWrite(green_pin_, max_brightness))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
      analogueWrite(green_pin_, max_brightness - increment))
      .InSequence(s1);

  // and bottom edge
  EXPECT_CALL(mock_arduino_class_,analogueWrite(green_pin_, remainder + increment))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(green_pin_, remainder))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(green_pin_, 0))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(green_pin_, increment))
      .InSequence(s2);

  my_led_.set_colour(LEDColour::GREEN); //set green
  my_led_.set_pulse(increment); //set pulse
  for (uint8_t i =0; i <= iterations; i++) {
    my_led_.pulse();
  }
}

TEST_F(MyLEDTest, PulseTestRed) {

  using testing::Sequence;
  using testing::_;
  using testing::AnyNumber;

  Sequence s1, s2;

  int8_t increment = 7; // odd number to really test method
  uint8_t max_brightness = UINT8_MAX;
  uint8_t iterations = 90;

  uint8_t  remainder = max_brightness % increment;


  // do we get calls?
  EXPECT_CALL(mock_arduino_class_, analogueWrite(_, _))
      .Times(AnyNumber());

  // Test top edge
  EXPECT_CALL(mock_arduino_class_,analogueWrite(red_pin_, max_brightness - remainder - increment))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness - remainder))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness - increment))
      .InSequence(s1);

  // and bottom edge
  EXPECT_CALL(mock_arduino_class_,analogueWrite(red_pin_, remainder + increment))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(red_pin_, remainder))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(red_pin_, 0))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(red_pin_, increment))
      .InSequence(s2);

  my_led_.set_colour(LEDColour::RED); //set red
  my_led_.set_pulse(increment); //set pulse
  for (uint8_t i =0; i <= iterations; i++) {
    my_led_.pulse();
  }
}

TEST_F(MyLEDTest, PulseTestYellow) {

  using testing::Sequence;
  using testing::_;
  using testing::AnyNumber;

  Sequence s1, s2, s3, s4;

  int8_t increment = 7; // odd number to really test method
  uint8_t max_brightness = UINT8_MAX;
  uint8_t iterations = 90;

  uint8_t  remainder = max_brightness % increment;


  // do we get calls?
  EXPECT_CALL(mock_arduino_class_, analogueWrite(_, _))
      .Times(AnyNumber());

  // Test top edge

  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(green_pin_, max_brightness - remainder - increment))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(green_pin_, max_brightness - remainder))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(green_pin_, max_brightness))
      .InSequence(s1);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(green_pin_, max_brightness - increment))
      .InSequence(s1);

  // need red and green pins for yellow
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness - remainder - increment))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness - remainder))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness))
      .InSequence(s2);
  EXPECT_CALL(mock_arduino_class_,
              analogueWrite(red_pin_, max_brightness - increment))
      .InSequence(s2);

  // and bottom edge
  EXPECT_CALL(mock_arduino_class_,analogueWrite(green_pin_, remainder + increment))
      .InSequence(s3);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(green_pin_, remainder))
      .InSequence(s3);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(green_pin_, 0))
      .InSequence(s3);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(green_pin_, increment))
      .InSequence(s3);

  EXPECT_CALL(mock_arduino_class_,analogueWrite(red_pin_, remainder + increment))
      .InSequence(s4);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(red_pin_, remainder))
      .InSequence(s4);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(red_pin_, 0))
      .InSequence(s4);
  EXPECT_CALL(mock_arduino_class_, analogueWrite(red_pin_, increment))
      .InSequence(s4);

  my_led_.set_colour(LEDColour::YELLOW); //set green
  my_led_.set_pulse(increment); //set pulse
  for (uint8_t i =0; i <= iterations; i++) {
    my_led_.pulse();
  }
}
