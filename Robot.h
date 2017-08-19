
#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include "NewPing.h"
#include "Servo.h"
//#include "Adafruit_PCD8544.h"
//#include "Adafruit_GFX.h"

/* Direction constants */
#define FRONT 0
#define RIGHT 1
#define LEFT 2
#define NO_WAY 3
#define ROTATION_DIST 15

class Robot {
public:
    //(90 / speed) * 1000 is the time to make a 90 degrees rotation
    //_speed/2 is due to brake time variation
    Robot(uint8_t _speed = 100): _delay(90000/_speed - _speed/2), safeDistance(15*_speed/100){
      setSpeed(_speed);
    }
    void init();
    void setSpeed(uint8_t);
    uint16_t getDelay() {return _delay;}
    void brake(bool stop);
    void goForward();
    void goBackward();
    void turnRight();
    void turnLeft();
    void rotate(uint8_t direction, uint16_t deg);
    void rightAngleRotation(uint8_t);
    void fullRotation();
    void halfRotation(uint8_t);

    void servoRotation(uint8_t);

    uint8_t readDistanceCM();
    uint8_t readDistanceIN();
    uint8_t mediumDistanceCM(uint8_t);
    uint8_t mediumDistanceIN(uint8_t);
    bool isClear();
    //bool stuck(uint16_t delay_time){return delay_time<=10;}/*{return !emptyDir && cmpPrevDir();}*/
    void findSafeZone();
    void setPath();

    void followLine();

    void setServo(bool);

    //void setScreen();

private:
    uint8_t speed;
    uint16_t _delay;
    uint8_t safeDistance;
    static const uint8_t maxSpeed = 255;
    uint8_t controlSpeed(uint8_t);
    void rotateOn(uint8_t, uint16_t);
    /*  Motor settings */
    static const uint8_t directionRPin = 12; /* Pin to control the direction of right motor, linked to Ch.A */
    static const uint8_t pwmRPin = 3; /* Pin to control pwm of the right motor */
    static const uint8_t brakeRPin = 9; /* Pin to brake right motor */

    static const uint8_t directionLPin = 13; /* Pin to control the direction of left motor, linked to Ch.B */
    static const uint8_t pwmLPin = 11; /* Pin to control pwm of the left motor */
    static const uint8_t brakeLPin = 8; /* Pin to brake left motor */

    static const uint8_t midLineFollower = 5;
    static const uint8_t rightLineFollower = 6;
    static const uint8_t leftLineFollower = 7;

    static const uint8_t servoPin = 10;

    Servo servo; /* Servo object to control the servo motor */
    uint8_t findPath();

    static const uint8_t trigPin = A1; /* It can be changed */
    static const uint8_t echoPin = A2; /* It can be changed */
    NewPing sonar; /* NewPing object to control the ultrasonic sensor, sets with trig and echo pins and maximum distance*/

    /* LCD pins*/
    static const uint8_t clkPin = 4;
    static const uint8_t dinPin = 2;
    static const uint8_t dcPin = 0;
    static const uint8_t resetPin = 1;

    //Adafruit_PCD8544 lcd;/*Adafruit_PCD8544 object to manage lcd screen*/
};


#endif // ROBOT_H_INCLUDED
