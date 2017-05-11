#include "Robot.h"

/******************* CONSTRUCTOR *******************/
Robot::Robot () {
  setSpeed(50);
  servo.attach(13);
  pinMode(directionRPin, OUTPUT);
  pinMode(pwmRPin, OUTPUT);
  pinMode(brakeRPin, OUTPUT);
  pinMode(directionLPin, OUTPUT);
  pinMode(pwmLPin, OUTPUT);
  pinMode(brakeLPin, OUTPUT);
}

Robot::Robot(uint8_t _speed) {
  setSpeed(_speed);
  servo.attach(13);
  pinMode(directionRPin, OUTPUT);
  pinMode(pwmRPin, OUTPUT);
  pinMode(brakeRPin, OUTPUT);
  pinMode(directionLPin, OUTPUT);
  pinMode(pwmLPin, OUTPUT);
  pinMode(brakeLPin, OUTPUT);
}

/******************* MOVEMENT *******************/
uint8_t Robot::controlSpeed(uint8_t speed) {
  if(speed > maxSpeed)
  return maxSpeed;
  return speed;
}

void Robot::setSpeed(uint8_t speed) {
  speed = controlSpeed(speed);
  analogWrite(pwmRPin, speed);
  analogWrite(pwmLPin, speed);
}

void Robot::brake(bool stop) { /*  */
  if (stop == true) {
    digitalWrite(brakeRPin, HIGH);
    digitalWrite(brakeLPin, HIGH);
  } else {
    digitalWrite(brakeRPin, LOW);
    digitalWrite(brakeLPin, LOW);
  }
}

void Robot::goForward() {
  brake(false);
  digitalWrite(directionRPin, HIGH);
  digitalWrite(directionLPin, HIGH);
}

void Robot::goBackward() {
  digitalWrite(directionRPin, LOW);
  digitalWrite(directionLPin, LOW);
}

void Robot::turnRight() {
  digitalWrite(directionRPin, HIGH);
  digitalWrite(brakeLPin, LOW);
  delay(50); /* Tempo provvisorio */
}

void Robot::turnLeft() {
  digitalWrite(directionLPin, HIGH);
  digitalWrite(brakeRPin, LOW);
  delay(50); /* Tempo provvisorio */
}

void Robot::rotateOn(uint8_t direction, uint16_t ms) {
  if (direction == LEFT) {
    digitalWrite(directionRPin, LOW);
    digitalWrite(directionLPin, HIGH);
  } else {
    digitalWrite(directionRPin, HIGH);
    digitalWrite(directionLPin, LOW);
  }
  delay(ms);
}

void Robot::rightAngleRotation(uint8_t direction) {
  rotateOn(direction, 50); /* Tempo provvisorio */
}

void Robot::halfRotation() {
  rotateOn(RIGHT, 100); /* Tempo provvisorio (2* tempo right) */
}

void Robot::fullRotation() {
  rotateOn(RIGHT, 200); /* Tempo provvisorio (2 * tempo half)*/
}

/******************* SERVO *******************/
void Robot::servoRotation(uint8_t degrees) { /* set servo position */
  servo.write(degrees);
  delay(100); /* wait 100 milliseconds to permit to the servo to reach the position */
}

/******************* ULTRASONIC *******************/
uint8_t Robot::readDistanceCM() { /* return the distance (in centimeters) read by the ultrasonic sensor */
  return sonar.ping_cm();
}

uint8_t Robot::readDistanceIN() { /* return the distance (in inches) read by the ultrasonic sensor */
  return sonar.ping_in();
}

bool Robot::isClear() { /* return if the path is clear or not */
    if(readDistanceCM() <= safeDistance)
        return false;
    return true;
}

/******************* OBLASTACLE AVOIDING *******************/
uint8_t Robot::findPath() {
  uint8_t leftDistance = 0;
  uint8_t frontDistance = 0;
  uint8_t rightDistance = 0;

  /* Imposto la posizione base del servo */
  uint8_t position = 45;

  /* Misuro la distanza variando progressivamente la posizione del servo */
  do {
    servoRotation(position);
    switch (position) {
      case 45: leftDistance = readDistanceCM(); break;
      case 90: frontDistance = readDistanceCM(); break;
      case 135: rightDistance = readDistanceCM(); break;
    }
    position += 45;
  } while (position <= 135);

  /* Ritorno la strada migliore */
  if (rightDistance > frontDistance) {
    if (rightDistance > leftDistance)
      return RIGHT;
    else
      return LEFT;
  } else {
    if(leftDistance > frontDistance)
      return LEFT;
  }
  return FRONT;
}

void Robot::setPath() {

}

void Robot::changePath() {

}
