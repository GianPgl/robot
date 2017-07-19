
#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include "NewPing.h"
#include "Servo.h"

/* Direction constants */
#define FRONT 0
#define RIGHT 1
#define LEFT 2
#define NO_WAY 3
#define DELAY 50

class Robot {
public:
    /*Robot(): speed(50), sonar(trigPin, echoPin, 200), obstacle(false) {
      pinMode(midLineFollower,INPUT);
      pinMode(rightLineFollower,INPUT);
      pinMode(leftLineFollower,INPUT);
    };*/
    Robot(uint8_t _speed = 50): speed(_speed)/*, sonar(trigPin, echoPin, 200), obstacle(false)*/ {
      /*pinMode(midLineFollower,INPUT);
      pinMode(rightLineFollower,INPUT);
      pinMode(leftLineFollower,INPUT);*/
    };
    void init();
    void setSpeed(uint8_t);
    void brake(bool stop);
    void goForward();
    void goBackward();
    void turnRight();
    void turnLeft();
    void rightAngleRotation(uint8_t);
    void fullRotation();
    void halfRotation();

    void servoRotation(uint8_t);

    uint8_t readDistanceCM();
    uint8_t readDistanceIN();
    bool isClear();

    uint8_t findPath();
    void setPath();
    //void changePath();

    void followLine();

    void setServo(bool);

private:
    int speed;
    static const uint8_t maxSpeed = 255;
    static const uint8_t halfSpeed = 127;
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

    uint8_t trigPin = A1; /* It can be changed */
    uint8_t echoPin = A2; /* It can be changed */
    NewPing sonar; /* NewPing object to control the ultrasonic sensor, sets with trig and echo pins and maximum distance*/

    static const uint8_t dangerDistance = 20;
    static const uint8_t safeDistance = 20;
    bool obstacle;
};


#endif // ROBOT_H_INCLUDED
