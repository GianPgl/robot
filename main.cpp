#include <Arduino.h>
#include "Robot.h"

Robot robot;

/*void setServo(uint8_t pin){
  robot.servo.attach(pin);
}*/

void setup(){
  robot.setServo();
}

void loop(){
      robot.setPath();
}
