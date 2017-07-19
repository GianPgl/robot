#include "Robot.h"

/******************* SETUP *******************/

void Robot::init(){
    pinMode(directionRPin, OUTPUT);
    pinMode(pwmRPin, OUTPUT);
    pinMode(brakeRPin, OUTPUT);
    pinMode(directionLPin, OUTPUT);
    pinMode(pwmLPin, OUTPUT);
    pinMode(brakeLPin, OUTPUT);
    pinMode(midLineFollower,INPUT);
    pinMode(rightLineFollower,INPUT);
    pinMode(leftLineFollower,INPUT);
    analogWrite(pwmLPin, speed);
    analogWrite(pwmRPin, speed);
    sonar.init(trigPin, echoPin);
    brake(true);
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

void Robot::brake(bool stop) { //true to stop, false to let the robot move
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
  /*Serial.begin(9600);
  Serial.println("I'm going forward!");
  Serial.end();*/
}

void Robot::goBackward() {
  brake(false);
  digitalWrite(directionRPin, LOW);
  digitalWrite(directionLPin, LOW);
}

void Robot::turnRight() {
  brake(false);
  digitalWrite(directionRPin, HIGH);
  digitalWrite(brakeLPin, LOW);
  /*Serial.begin(9600);
  Serial.println("I'm going right!");
  Serial.end();*/
}

void Robot::turnLeft() {
  brake(false);
  digitalWrite(directionLPin, HIGH);
  digitalWrite(brakeRPin, LOW);
  /*Serial.begin(9600);
  Serial.println("I'm going left!");
  Serial.end();*/
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
  rotateOn(direction, DELAY); /* Tempo provvisorio */
}

void Robot::halfRotation() {
  rotateOn(RIGHT, 2*DELAY); /* Tempo provvisorio (2* tempo right) */
}

void Robot::fullRotation() {
  rotateOn(RIGHT, 4*DELAY); /* Tempo provvisorio (2 * tempo half)*/
}

/******************* SERVO *******************/

void Robot::setServo(bool state){
  if(state == true){
    if(servo.attached())
      return;
    servo.attach(servoPin);
    servo.write(90);
  } else {
    if(!servo.attached())
      return;
    servo.detach();
  }
}

void Robot::servoRotation(uint8_t degrees) { /* set servo position */
  if(!servo.attached())
    this->setServo(true);
  servo.write(degrees);
  delay(100); /* wait 100 milliseconds to permit to the servo to reach the position */
}

/******************* ULTRASONIC *******************/
uint8_t Robot::readDistanceCM() { /* return the distance (in cm) read by the ultrasonic sensor */
  uint8_t distance = sonar.ping_cm();
  return distance;
}

uint8_t Robot::readDistanceIN() {
  uint8_t distance = sonar.ping_in();
  return distance;
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
  if(!servo.attached())
    this->setServo(true);
  /* Misuro la distanza variando progressivamente la posizione del servo */
  do {
    servoRotation(position);
    switch (position) {
      case 45: leftDistance = readDistanceCM(); break;
      case 90: frontDistance = readDistanceCM(); break;
      case 135: rightDistance = readDistanceCM(); break;
    }
    position += 45;
    delay(400);
  } while (position <= 135);

  /* Ritorno la strada migliore */
  if (rightDistance > frontDistance) {
    if (rightDistance > leftDistance)
      return rightDistance < safeDistance ? NO_WAY : RIGHT;
    else
      return leftDistance < safeDistance ? NO_WAY : LEFT;
  } else {
    if(leftDistance > frontDistance)
      return leftDistance < safeDistance ? NO_WAY : LEFT;
  }
  return frontDistance < safeDistance ? NO_WAY : FRONT;
}

void Robot::setPath() {
  uint8_t direction = findPath();
  if(direction == NO_WAY){
    halfRotation();
    findPath();
  }
  else{
    switch(direction){
      case FRONT:
        goForward();
      break;
      case RIGHT:
        turnRight();
      break;
      case LEFT:
        turnLeft();
      break;
    }
  }
}

void Robot::followLine(){
  //Serial.begin(9600);
  //if(!digitalRead(midLineFollower)){  //if a black line is detected input is LOW
  //  goForward();
  //}
  if(!digitalRead(rightLineFollower)){  //if a black line is detected input is LOW
    turnRight();
    //Serial.write("Destra");
    return;
  }
  if(!digitalRead(leftLineFollower)){  //if a black line is detected input is LOW
    turnLeft();
    //Serial.write("Sinistra");
    return;
  }
  if(/*digitalRead(midLineFollower) && */digitalRead(rightLineFollower) && digitalRead(leftLineFollower)){
    //espressione triste
  }
  //Serial.end();
}
