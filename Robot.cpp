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
    pinMode(leftLightSensor, INPUT);
    pinMode(rightLightSensor, INPUT);
    brake(true);
    analogWrite(pwmLPin, speed);
    analogWrite(pwmRPin, speed);
    sonar.init(trigPin, echoPin);
    setServo(true);
    setServo(false);
    //lcd.init(clkPin, dinPin, dcPin, resetPin);
}

/******************* MOVEMENT *******************/
uint8_t Robot::controlSpeed(const uint16_t *_speed) {
  if(*_speed > maxSpeed)
    return maxSpeed;
  return *_speed;
}

void Robot::setSpeed(uint16_t _speed) {
  speed = controlSpeed(&_speed);
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

void Robot::rotateOn(u_char direction, uint16_t deg) {
  if (direction == RIGHT) {
    turnRight();
  } else {
    turnLeft();
  }
  delay(map(deg, 0, 360, 0, 4*getDelay()));
  brake(true);
}

void Robot::rightAngleRotation(u_char direction) {
  rotateOn(direction, 90);
}

void Robot::rotate(u_char direction, uint16_t deg){
  uint16_t _deg = deg > 360 ? 360 : deg;  //needs more than a full rotation if deg > 360, so it does a stepper one
  rotateOn(direction, _deg);
  if(deg > 360){
    rotateOn(direction, deg - 360);
  }
}

void Robot::halfRotation(u_char direction) {
  rotateOn(direction, 180);
}

void Robot::fullRotation(u_char direction) {
  rotateOn(direction, 360);
}

/******************* SERVO *******************/

void Robot::setServo(bool state){
  if(state == true){
    if(servo.attached())
      return;
    servo.attach(servoPin);
    servoRotation(90-20);
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
  delay(400); /* wait 100 milliseconds to permit to the servo to reach the position */
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

uint8_t Robot::mediumDistanceCM(uint8_t it){
  uint16_t total = 0;
  for(uint8_t i=0; i<it; i++){
    total += readDistanceCM();
  }
  return total/it;
}

uint8_t Robot::mediumDistanceIN(uint8_t it){
  uint16_t total = 0;
  for(uint8_t i=0; i<it; i++){
    total += readDistanceIN();
  }
  return total/it;
}

bool Robot::isClear(bool line = false) { /* checks path clearness */
  uint8_t _safeDistance = line == true ? 10 : safeDistance;
  uint8_t distance = readDistanceCM();
    if(distance >= _safeDistance)
        return true;
    return false;
}

/******************* OBLASTACLE AVOIDANCE *******************/


uint8_t Robot::getLineDir(){
  return !digitalRead(rightLineFollower) ? RIGHT
  : !digitalRead(leftLineFollower) ? LEFT
  : !digitalRead(midLineFollower) ? FRONT : NO_WAY;
}

uint8_t Robot::findPath() {
  uint8_t leftDistance = 0;
  uint8_t frontDistance = 0;
  uint8_t rightDistance = 0;
  uint8_t min;

  /* Imposto la posizione base del servo */
  uint8_t position = 45-20;
  setServo(true);
  /* Misuro la distanza variando progressivamente la posizione del servo */
  do {
    servoRotation(position);
    switch (position) {
      case 45-20: leftDistance = mediumDistanceCM(10); break;
      case 90-20: frontDistance = mediumDistanceCM(10); break;
      case 135-20: rightDistance = mediumDistanceCM(10); break;
    }
    position += 45;
  } while (position <= 135-20);
  min = min(frontDistance, min(rightDistance, leftDistance));
  if(min < safeDistance)
    return NO_WAY;
  if(min == frontDistance)
    return FRONT;
  else{
    if(min == rightDistance)
      return RIGHT;
    return LEFT;
  }
  /*if (rightDistance > frontDistance) {
    if (rightDistance > leftDistance)
      return rightDistance < safeDistance ? NO_WAY : RIGHT;
    else
      return leftDistance < safeDistance ? NO_WAY : LEFT;
  } else {
    if(leftDistance > frontDistance)
      return leftDistance < safeDistance ? NO_WAY : LEFT;
    else
      return frontDistance < safeDistance ? NO_WAY : FRONT;
  }*/
}

void Robot::findSafeZone(){
  uint8_t right, left;  //get rotation direction or go back if there's no way to ratate
  servoRotation(0);
  right = mediumDistanceCM(10);
  servoRotation(180-20);
  left = mediumDistanceCM(10);
  if(right < ROTATION_DIST && left < ROTATION_DIST){
    servoRotation(90-20);
    goBackward();
    while(readDistanceCM() < safeDistance -1)  //last position from which it can't go forward and get stuck again
      delay(50);
    brake(true);
  }
  else{
    servoRotation(90-20);
    rotate(right >= left ? RIGHT : LEFT, 90);
  }
}

void Robot::setPath() {
  u_char direction = findPath();
  while(direction == NO_WAY){
    findSafeZone();
    direction = findPath();
  }
  servoRotation(90-20);
  if(direction != FRONT){
    if(direction == RIGHT)
      turnRight();
    else
      turnLeft();
    while(!isClear())
      delay(50);
    brake(true);
    if(isClear()){             //  1)
      goForward();
      while(isClear())
        delay(50);
      brake(true);
    }
  }
  else{
    if(isClear()){            //  2)
      goForward();
      while(isClear())
        delay(50);
      brake(true);
    }                         //  1) e 2) sono uguali ma se non li scrivessi due volte non potrei usare l'else
    else
      findSafeZone();
  }
}

void Robot::followLine(){
  u_char dir = getLineDir();
  u_char prevDir = dir;
  while(true){
    switch(dir){
      case FRONT:
        goForward();
        while((dir = getLineDir())==FRONT);
        brake(true);
        break;
      case RIGHT:
        turnRight();
        prevDir = dir;
        while((dir = getLineDir())!=FRONT);
        brake(true);
        break;
      case LEFT:
        turnLeft();
        prevDir = dir;
        while((dir = getLineDir())!=FRONT);
        brake(true);
        break;
      case NO_WAY:
        if(prevDir != NO_WAY)
          prevDir == RIGHT ? turnRight() : turnLeft();
        while((dir = getLineDir()) == NO_WAY);
        brake(true);
        break;
    }
  }
}

void Robot::followAvoidServo(){
  u_char dir = getLineDir();
  u_char prevDir = dir;
  setServo(true);
  while(true){
    if(isClear(true)){
      switch(dir){
        case FRONT:
          servoRotation(90-20);
          goForward();
          while(isClear(true) && (dir = getLineDir())==FRONT);
          brake(true);
          break;
        case RIGHT:
          servoRotation(135-20);
          turnRight();
          prevDir = dir;
          while(isClear(true) && (dir = getLineDir())!=FRONT);
          brake(true);
          break;
        case LEFT:
          servoRotation(45-20);
          turnLeft();
          prevDir = dir;
          while(isClear(true) && (dir = getLineDir())!=FRONT);
          brake(true);
          break;
        case NO_WAY:
          if(prevDir != NO_WAY){
            prevDir == RIGHT ? servoRotation(135-20) : servoRotation(90-20);
            prevDir == RIGHT ? turnRight() : turnLeft();
          }
          while(isClear(true) && (dir = getLineDir()) == NO_WAY);
          brake(true);
          break;
      }
    }
  }
}

void Robot::followAvoid(){
  u_char dir = getLineDir();
  u_char prevDir = dir;
  setServo(true);
  while(true){
    if(isClear(true)){
      switch(dir){
        case FRONT:
          goForward();
          while(isClear(true) && (dir = getLineDir())==FRONT);
          brake(true);
          break;
        case RIGHT:
          turnRight();
          prevDir = dir;
          while(isClear(true) && (dir = getLineDir())!=FRONT);
          brake(true);
          break;
        case LEFT:
          turnLeft();
          prevDir = dir;
          while(isClear(true) && (dir = getLineDir())!=FRONT);
          brake(true);
          break;
        case NO_WAY:
          if(prevDir != NO_WAY)
            prevDir == RIGHT ? turnRight() : turnLeft();
          while(isClear(true) && (dir = getLineDir()) == NO_WAY);
          brake(true);
          break;
      }
    }
  }
}

/******************LIGHT SENSORS*******************/

uint8_t Robot::lightIntensity(u_char dir = FRONT){
  if(dir != RIGHT && dir!= LEFT && dir != FRONT)
    return analogRead(midLightSensor);
  return dir == FRONT? analogRead(midLightSensor) :
    dir == RIGHT ? analogRead(rightLightSensor) : analogRead(leftLightSensor);
}

void Robot::setEnvLight(){
  lightCalibration = false;
  uint16_t temp = lightIntensity();
  envLight = (temp > 255 ? 255 : temp);
}

uint8_t Robot::getLightDir(){
  uint8_t right = lightIntensity(RIGHT);
  uint8_t left = lightIntensity(LEFT);
  uint8_t front = lightIntensity(FRONT);
  uint8_t min;
  uint8_t envLight = getEnvLight();
  Serial.print("front: ");
  Serial.print(front);
  Serial.print("\tleft: ");
  Serial.print(left);
  Serial.print("\tright: ");
  Serial.println(right);
  /*if(right < left){
    if(right >= envLight-ENV_TOLERANCE)
      return NO_WAY;
  } else {
    if(left >= envLight-ENV_TOLERANCE)
      return NO_WAY;
  }
  if(abs(right-left) <= 10)
    return FRONT;
  if(right < left)
    return RIGHT;
  return LEFT;*/
  min = min(front, min(left, right));
  if(min >= envLight/*-ENV_TOLERANCE*/)
    return NO_WAY;
  if(min == front)
    return FRONT;
  else{
    if(min == right)
      return RIGHT;
    return LEFT;
    }
}

void Robot::followLight(){
  if(envLight == 0 || lightCalibrationRequired())
    setEnvLight();
  Serial.print("env: ");
  Serial.print(getEnvLight());
  Serial.print("\n");
  u_char dir = getLightDir();
  //while(true){
    switch(dir){
      case NO_WAY:
      //while((dir = getLightDir(envLight))==NO_WAY)
      //  delay(10);
        break;
      case LEFT:
        turnLeft();
        while((dir = getLightDir())==LEFT)
          delay(10);
        brake(true);
        break;
      case RIGHT:
        turnRight();
        while((dir = getLightDir())==RIGHT)
          delay(10);
        brake(true);
        break;
      case FRONT:
        goForward();
        while((dir = getLightDir())==FRONT)
          delay(10);
        brake(true);
        break;
    }
  //}
}

/**************************LCD*********************/

/*void Robot::setScreen(){
  lcd.begin();
  //lcd.display();
}*/
