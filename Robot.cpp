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
    //lcd.init(clkPin, dinPin, dcPin, resetPin);
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
}

void Robot::goBackward() {
  brake(false);
  digitalWrite(directionRPin, LOW);
  digitalWrite(directionLPin, LOW);
}

void Robot::turnLeft() {
  brake(false);
  digitalWrite(directionRPin, HIGH);
  digitalWrite(directionLPin, LOW);
}

void Robot::turnRight() {
  brake(false);
  digitalWrite(directionLPin, HIGH);
  digitalWrite(directionRPin, LOW);
}

void Robot::rotateOn(uint8_t direction, uint16_t deg) {
  brake(false);
  if (direction == LEFT) {
    digitalWrite(directionRPin, LOW);
    digitalWrite(directionLPin, HIGH);
  } else {
    digitalWrite(directionRPin, HIGH);
    digitalWrite(directionLPin, LOW);
  }
  delay(map(deg, 0, 360, 0, 4*getDelay()));
  brake(true);
}

void Robot::rightAngleRotation(uint8_t direction) {
  rotateOn(direction, 90);
}

void Robot::rotate(uint8_t direction, uint16_t deg){
  uint16_t _deg = deg > 360 ? 360 : deg;  //needs more than a full rotation if deg > 360, so it does a stepper one
  rotateOn(direction, _deg);
  if(deg > 360){
    rotateOn(direction, deg - 360);
  }
}

void Robot::halfRotation(uint8_t direction) {
  rotateOn(direction, 180);
}

void Robot::fullRotation() {
  rotateOn(RIGHT, 360);
}

/******************* SERVO *******************/

void Robot::setServo(bool state){
  if(state == true){
    if(servo.attached())
      return;
    servo.attach(servoPin);
    servo.write(90-20);
  } else {
    if(!servo.attached())
      return;
    servo.detach();
  }
}

void Robot::servoRotation(uint8_t degrees) { /* set servo position */
  if(!servo.attached())
    setServo(true);
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

bool Robot::isClear() { /* checks path clearness */
  uint8_t distance = readDistanceCM();
    if(distance >= safeDistance)
        return true;
    return false;
}

/******************* OBLASTACLE AVOIDANCE *******************/

void Robot::updateDir(uint8_t newDir){
    prevDir[0] = prevDir[1];
    prevDir[1] = currDir[0];
    currDir[0] = currDir[1];
    currDir[1] = newDir;
    if(prevDir[0] != -1)
      emptyDir = false;
}

inline void Robot::resetDir(){
  prevDir[0]=prevDir[1]=currDir[0]=currDir[1]=-1;
  emptyDir = true;
}

uint8_t Robot::findPath() {
  uint8_t leftDistance = 0;
  uint8_t frontDistance = 0;
  uint8_t rightDistance = 0;

  /* Imposto la posizione base del servo */
  uint8_t position = 45-20;
  if(!servo.attached())
    setServo(true);
  /* Misuro la distanza variando progressivamente la posizione del servo */
  do {
    servoRotation(position);
    switch (position) {
      case 45-20: leftDistance = readDistanceCM(); break;
      case 90-20: frontDistance = readDistanceCM(); break;
      case 135-20: rightDistance = readDistanceCM(); break;
    }
    position += 45;
    delay(400);
  } while (position <= 135-20);
  if (rightDistance > frontDistance) {
    if (rightDistance > leftDistance)
      return rightDistance < safeDistance ? NO_WAY : RIGHT;
    else
      return leftDistance < safeDistance ? NO_WAY : LEFT;
  } else {
    if(leftDistance > frontDistance)
      return leftDistance < safeDistance ? NO_WAY : LEFT;
    else
      return frontDistance < safeDistance ? NO_WAY : FRONT;
  }
}

void Robot::findSafeZone(){
  uint8_t right, left;  //get rotation direction or go back if there's no way to ratate
  servoRotation(0);
  delay(400);
  right = readDistanceCM();
  servoRotation(180-20);
  delay(400);
  left = readDistanceCM();
  if(right < ROTATION_DIST && left < ROTATION_DIST){
    servoRotation(90-20);
    goBackward();
    while(readDistanceCM() < safeDistance -1)
      delay(10); //last position from which it can't go forward and get stuck again
    brake(true);
  }
  else{
    servoRotation(90-20);
    halfRotation(right >= left ? RIGHT : LEFT); //both-NO_ECHO case means RIGHT
  }
  resetDir();
}

void Robot::setPath() {
  uint8_t direction = findPath();
  while(direction == NO_WAY){
    findSafeZone();
    direction = findPath();
  }
  if(direction != FRONT){
    updateDir(direction);
    if(stuck()){
      findSafeZone();
      direction = findPath();
    }
    rotate(direction == RIGHT ? RIGHT : LEFT ,45);
  }
  servoRotation(90-20);
  delay(400);
  if(isClear()){
    goForward();
    while(isClear())
      delay(10);
    brake(true);
    resetDir();
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

/**************************LCD*********************/

/*void Robot::setScreen(){
  lcd.begin();
  //lcd.display();
}*/
