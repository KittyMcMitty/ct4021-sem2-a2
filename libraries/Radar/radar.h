// include guard to prevent multiple definition; Stroustrup §15.3.3
#ifndef RADAR_H
#define RADAR_H

#ifndef UNIT_TEST
#include <Servo.h>
#endif // UNIT_TEST

#include <ArduinoInterface.h>

#define SPEED_OF_SOUND 0.343 // speed of sound in mm/µs

/*
 * Radar - encapsulates servo motor and distance sensor.
 * This class does two things - move the servo motor and provide distance
 * readings. A call to move() will increment the servo by 1 degree, and turn
 * the servo around if maximum rotation is reached. ping() will return a
 * distance in millimeters.
 *
 * Template parameters:
 *
 * class ServoInterface - Either a concrete or mock Servo object
 */
template <class ServoInterface, class ArduinoInterface>
class Radar
{
 private:
  static const int8_t radar_l_ = -1;  // using constant here instead of macro
                                      // as per Google style guide
  static const int8_t radar_r_ = 1;   // https://google.github.io/styleguide/cppguide.html#Preprocessor_Macros
  int8_t direction_ {radar_l_};       // Radar will track whether servo needs
                                      // to be going left or right
  uint8_t servo_angle_ {90};          // Angle of servo in range 0 <= angle <= 180
  ServoInterface* servo_;             // servo object - either concrete or mock
  uint8_t trigger_pin_ {0};           // trigger pin for sensor
  uint8_t echo_pin_ {0};              // echo pin for sensor
 public:

  /*
   * Default constructor - used in release
   */
  Radar() {
    ServoInterface servo;
    servo_ = &servo;
  };

  /*
   * Pass in servo object. Used for testing
   *
   * Arguments:
   *
   * ServoInterface* servo - An object adhering to the ServoInterface type
   * passed as template parameter.
   */
  explicit Radar(ServoInterface* servo) {
      servo_ = servo;
  };

  /*
   * Init - initialise radar object
   *
   * Arguments:
   *
   * uint8_t trigger_pin  - Trigger pin on ultrasonic sensor
   * uint8_t echo_pin     - Echo pin on ultrasonic sensor
   * uint8_t servo_pin    - Pin used by servo motor
   */
  void init(uint8_t trigger_pin, uint8_t echo_pin, uint8_t servo_pin) {
    trigger_pin_ = trigger_pin; // save trigger and echo for ultrasonic sensor
    echo_pin_ = echo_pin;

    ArduinoInterface::pinMode(trigger_pin_, OUTPUT); // set the pin modes for sensor
    ArduinoInterface::pinMode(echo_pin_, INPUT);

    servo_->attach(servo_pin); // attach servo

    servo_->write(servo_angle_); // set servo initial position
  };

  /*
   * Move - move the servo
   *
   * This method moves the servo by 1° with each call. When maximum rotation is
   * attained, the angle will reverse and subsequent calls will increment the
   * servo in the opposite direction.
   */
  uint8_t move() {
    // if servo is at minimum angle...
    if (servo_angle_ == 0) {
      // start turning right
      direction_ = radar_r_;
    } else if (servo_angle_ == 180) { // maximum angle
      // start turning left
      direction_ = radar_l_;
    }

    servo_angle_ += direction_;
    servo_->write(servo_angle_);

    return servo_angle_;
  };

  /*
   * Ping - ping the radar and return range measurement.
   *
   * This method returns a range measurement from the ultrasonic sensor in mm.
   */
  uint32_t ping() {
    // make sure trigger is off...
    ArduinoInterface::digitalWrite(trigger_pin_, LOW);
    ArduinoInterface::delayMicroseconds(2); // wait 2µs

    // Send trigger set_pulse
    ArduinoInterface::digitalWrite(trigger_pin_, HIGH);
    ArduinoInterface::delayMicroseconds(10); // wait 10µs
    ArduinoInterface::digitalWrite(trigger_pin_, LOW);

    // get the duration of set_pulse just sent
    unsigned long duration = ArduinoInterface::pulseIn(echo_pin_, HIGH);

    return duration * SPEED_OF_SOUND / 2; // divide by 2 for there and back again
  };
};

#endif