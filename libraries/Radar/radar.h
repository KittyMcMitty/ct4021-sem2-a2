// include guard to prevent multiple definition; Stroustrup §15.3.3
#ifndef RADAR_H
#define RADAR_H

#ifdef UNIT_TEST
#include <Servo.h>
#else
#include <Servo.h>
#endif



#define SPEED_OF_SOUND 0.343 // speed of sound in mm/µs

/*
    Radar - encapsulates servo motor and distance sensor.

    This class does two things - move the servo motor and provide distance
    readings. A call to move() will increment the servo by 1 degree, and turn
    the servo around if maximum rotation is reached. ping() will return a 
    distance in millimeteres. 
*/
class Radar
{
private:   
    static const int8_t radar_l_ = -1; // using constant here instead of macro as per Google style guide 
    static const int8_t radar_r_ = 1;  // https://google.github.io/styleguide/cppguide.html#Preprocessor_Macros
    int8_t direction_;          // Radar will track whether servo needs to be going left or right
    uint8_t servo_angle_;       // Angle of servo in range 0 <= angle <= 180
    Servo servo_;               // servo object from servo.h
    uint8_t trigger_pin_;       // trigger pin for sensor
    uint8_t echo_pin_;          // echo pin for sensor
public:    
    //Radar (uint8_t trigger_pin, uint8_t echo_pin, uint8_t servo_pin);  // class constucter
    void init(uint8_t trigger_pin, uint8_t echo_pin, uint8_t servo_pin);
    uint8_t move();                // move the servo
    uint32_t ping();            // ping the radar and return range measurement.
};

#endif