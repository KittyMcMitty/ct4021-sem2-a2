#include <Arduino.h>
#include "radar.h"

const int8_t radar_l_; // Have to define static const data members here too
const int8_t radar_r_; 

void Radar::init(uint8_t trigger_pin, uint8_t echo_pin, uint8_t servo_pin)
{
    trigger_pin_ = trigger_pin; // save trigger and echo for ultrasonic sensor
    echo_pin_ = echo_pin;

    pinMode(trigger_pin_, OUTPUT); // set the pin modes for sensor
    pinMode(echo_pin_, INPUT);

    servo_.attach(servo_pin); // attach servo
    /*
    Serial.begin(9600);
    Serial.print("Servo Pin: ");
    Serial.println(servo_pin);
    */
    // servo init    
    direction_ = 1; 
    servo_angle_ = 90;
    servo_.write(servo_angle_); // set servo initial position
}

uint8_t Radar::move()
{
    // if servo is at minimum angle...
    if (servo_angle_ == 0) {
        // start turning right
        direction_ = radar_r_;
    } else if (servo_angle_ == 180) { // maximum angle
        // start turning left
        direction_ = radar_l_;
    }

    servo_angle_ += direction_;
    servo_.write(servo_angle_);

    return servo_angle_;
}

uint32_t Radar::ping()
{
    // make sure trigger is off...
    digitalWrite(trigger_pin_, LOW);
    delayMicroseconds(2); // wait 2µs

    digitalWrite(trigger_pin_, HIGH);
    delayMicroseconds(10); // wait 10µs
    digitalWrite(trigger_pin_, LOW);

    // get the duration of pulse just sent
    unsigned long duration = pulseIn(echo_pin_, HIGH);

    return duration * SPEED_OF_SOUND / 2; // divide by 2 for there and back again

}