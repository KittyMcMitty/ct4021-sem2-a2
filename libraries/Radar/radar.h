//
// Created by Nicholas Ives
//

// include guard to prevent multiple definition; Stroustrup §15.3.3
#ifndef RADAR_H
#define RADAR_H

#ifndef UNIT_TEST
#include <Servo.h>
#endif // UNIT_TEST

#include <ArduinoInterface.h>

#define SPEED_OF_SOUND 0.343 // speed of sound in mm/µs

// These globals are needed for the ping echo ISR
namespace EchoISR {
extern volatile uint32_t pulse_start_ ;
extern volatile uint32_t pulse_end_;
extern volatile bool i_flag;
extern uint8_t echo_pin_;  // echo pin for sensor

/*
 * Echo ISR
 *
 * This method measures the time when the echo pin is set either HIGH or LOW.
 * The ping method can then do the slower work of figuring out how long it took
 * and returning a value.
 */
void echo_isr();

} // namespace EchoISR

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
template <class ServoInterface>
class Radar
{
 private:
  static const int8_t radar_l_ = -1;

  static const int8_t radar_r_ = 1;
  int8_t direction_ {radar_l_};       // Radar will track whether servo needs
                                      // to be going left or right
  uint8_t servo_angle_ {90};          // Angle of servo in range 0 <= angle <= 180
  ServoInterface* servo_;             // servo object - either concrete or mock
  uint8_t trigger_pin_ {0};           // trigger pin for sensor

  inline void attach_echo_isr_();

 public:

  /*
   * Default constructor - used in release
   */
  Radar() {
    //using AI = ArduinoInterface;
    auto servo = new ServoInterface;
    servo_ = servo;
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
      //attach_echo_isr_();
  };

  void init(uint8_t trigger_pin, uint8_t echo_pin, uint8_t servo_pin);
  uint8_t   move();
  uint32_t  ping();
};

/*
 * Ping - ping the radar and return range measurement.
 *
 * This method returns a range measurement from the ultrasonic sensor in mm.
 * This method will return unreliable results if you call it too often! It can
 * take up to 500ms for a reading from the ultrasonic sensor, so calling it
 * more often than that will generate unreliable results.
 *
 * Each call will return the range measurement (if any) from the prior call.
 * The alternative would be just block for 500ms while we wait for a pulse.
 */
template<class ServoInterface>
uint32_t Radar<ServoInterface>::ping() {
  using AI = ArduinoInterface;
  using namespace EchoISR;

  // we're reading two multibyte values, so turn off interrupts
  // we're doing this at the start because when we pulse the trigger right after
  // there will definitely be an interrupt shortly afterwards
  uint32_t distance = UINT32_MAX;
  noInterrupts();
  // if we have times for both of these
  if ((pulse_start_ || pulse_end_) != 0){
    // micros() rolls over every 70mins roughly, so check for that
    if (pulse_end_ < pulse_start_) {
      distance = UINT32_MAX - pulse_start_ + pulse_end_ * SPEED_OF_SOUND / 2;
    } else if (pulse_end_ - pulse_start_ == 38){ // this is if the sensor detects nothing
      distance = UINT32_MAX;
    } else {
      distance = (pulse_end_ - pulse_start_) * SPEED_OF_SOUND / 2;
    }
    pulse_start_ = 0;
    pulse_end_ = 0;
  }
  interrupts(); // and turn them back on

  // make sure trigger is off...
  AI::digitalWrite(trigger_pin_, LOW);
  AI::delayMicroseconds(2); // wait 2µs

  // Send trigger pulse
  AI::digitalWrite(trigger_pin_, HIGH);
  AI::delayMicroseconds(10); // wait 10µs
  AI::digitalWrite(trigger_pin_, LOW);

  return distance;
}

template<class ServoInterface>
inline void Radar<ServoInterface>::attach_echo_isr_() {
  using namespace EchoISR;
  using AI = ArduinoInterface;
  AI::attachInterrupt(digitalPinToInterrupt(echo_pin_), echo_isr, CHANGE);
}

/*
 * Move - move the servo
 *
 * This method moves the servo by 1° with each call. When maximum rotation is
 * attained, the angle will reverse and subsequent calls will increment the
 * servo in the opposite direction.
 */
template<class ServoInterface>
uint8_t Radar<ServoInterface>::move()  {
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
}

/*
 * Init - initialise radar object
 *
 * Arguments:
 *
 * uint8_t trigger_pin  - Trigger pin on ultrasonic sensor
 * uint8_t echo_pin     - Echo pin on ultrasonic sensor
 * uint8_t servo_pin    - Pin used by servo motor
 */
template<class ServoInterface>
void Radar<ServoInterface>::init(uint8_t trigger_pin,
                                 uint8_t echo_pin,
                                 uint8_t servo_pin) {

  using AI = ArduinoInterface;

  trigger_pin_ = trigger_pin; // save trigger and echo for ultrasonic sensor
  EchoISR::echo_pin_ = echo_pin;

  AI::pinMode(trigger_pin_, OUTPUT); // set the pin modes for sensor
  AI::pinMode(EchoISR::echo_pin_, INPUT);

  attach_echo_isr_();

  servo_->attach(servo_pin); // attach servo

  servo_->write(servo_angle_); // set servo initial position
};

#endif