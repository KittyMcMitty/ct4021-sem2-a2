//
// Created by Nicholas Ives on 27/03/2020.
//

#include <gmock/gmock.h>
#include <radar.h>
#include <ArduinoInterface.h>

/*
 * MockServo - mock Servo class
 *
 * This is a test adapter. Only Servo methods used in Radar class are mocked here.
 */
class MockServo {
 public:
  MOCK_METHOD(uint8_t, attach, (uint8_t));
  MOCK_METHOD(void, write, (int));
};

class RadarTest : public ::testing::Test {
 protected:
  // pins for radar
  const static uint8_t trigger_ {0};
  const static uint8_t echo_ {0};
  const static uint8_t servo_ {0};

  // Mock fixtures
  MockServo mock_servo_;
  MockArduinoClass mock_arduino_class_;

  // Radar with mock adapters
  Radar<MockServo> radar_ {&mock_servo_};

  RadarTest() {
    MockArduino::mock = &mock_arduino_class_;
  }

};

TEST_F(RadarTest, InitTest) {

  // Does Servo.attach() get called with servo pin?
  EXPECT_CALL(mock_servo_, attach(servo_))
      .Times(1);

  // Does Servo.write() get called to set initial position?
  EXPECT_CALL(mock_servo_, write)
      .Times(1);

  // Does the echo pin get set to input?
  EXPECT_CALL(mock_arduino_class_, pinMode(echo_, INPUT))
      .Times(1);

  // Does the trigger pin get set to output?
  EXPECT_CALL(mock_arduino_class_, pinMode(trigger_, OUTPUT))
      .Times(1);

  radar_.init(trigger_, echo_, servo_);
}

TEST_F(RadarTest, MoveTest) {
  using ::testing::Sequence;
  using ::testing::_;
  using ::testing::AnyNumber;

  Sequence s1, s2;

  // Do we get any calls at all?
  EXPECT_CALL(mock_servo_, write(_))
      .Times(AnyNumber());

  // Two edge cases - when servo is fully left and fully right.

  // Do we get call of write in sequence 179, 180, 179
  EXPECT_CALL(mock_servo_, write(179))
      .InSequence(s1);
  EXPECT_CALL(mock_servo_, write(180))
      .InSequence(s1);
  EXPECT_CALL(mock_servo_, write(179))
      .InSequence(s1);

  // Do we get call of write in sequence 1, 0, 1
  EXPECT_CALL(mock_servo_, write(1))
      .InSequence(s2);
  EXPECT_CALL(mock_servo_, write(0))
      .InSequence(s2);
  EXPECT_CALL(mock_servo_, write(1))
      .InSequence(s2);

  // This is enough for both edge cases regardless of start position.
  for (int i =0; i < 361; i++){
    radar_.move();
  }
}

TEST_F(RadarTest, PingTest) {
  using ::testing::Sequence;
  using ::testing::_;
  using ::testing::Return;

  // All calls must be in sequence.
  Sequence s1;

  unsigned long value {10000}, return_value {0}, expected_value {0};

  // Do we get LOW write to trigger?
  EXPECT_CALL(mock_arduino_class_, digitalWrite(trigger_, LOW))
        .InSequence(s1);

  // Do we get a delay?
  EXPECT_CALL(mock_arduino_class_, delayMicroseconds(_))
        .InSequence(s1);

  // Do we get HIGH write to trigger?
  EXPECT_CALL(mock_arduino_class_, digitalWrite(trigger_, HIGH))
      .InSequence(s1);

  // Do we get a delay?
  EXPECT_CALL(mock_arduino_class_, delayMicroseconds(_))
      .InSequence(s1);

  // Do we get LOW write to trigger?
  EXPECT_CALL(mock_arduino_class_, digitalWrite(trigger_, LOW))
      .InSequence(s1);

  EXPECT_CALL(mock_arduino_class_, pulseIn(echo_, HIGH))
      .InSequence(s1)
      .WillOnce(Return(value));

  return_value = radar_.ping();
  expected_value = value * SPEED_OF_SOUND / 2;

  ASSERT_EQ(return_value, expected_value);
}