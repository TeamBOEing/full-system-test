/*
 * Course Navigate
 * Author: Cody Crossley
 * Date: 02 November 2015
 * Version: 1.0
 * 
 * This program will be used to test a BOEbot's ability to navigate toward a light.
 * 
 * ======= VERSION HISTORY =======
 * Version 1.0: Created file and basic functionality - CC - 02 November 2015
 */

#include <Servo.h>
#include "BOEbot.h"

const unsigned int SPEED = 250;
const unsigned int DRIVE_TIME = 500;
const unsigned int TURN_TIME = 50;
const unsigned int LIGHT_READ_SINGLE = 75;
const unsigned int LIGHT_READ_BOTH = 250;
const float LIGHT_RATIO_LEFT = 0.7;
const float LIGHT_RATIO_RIGHT = 1.3;

void driveForw(unsigned int spd, unsigned int time_length) {
  turnLeftMotorForward(spd);
  turnRightMotorForward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

void driveBack(unsigned int spd, unsigned int time_length) {
  turnLeftMotorBackward(spd);
  turnRightMotorBackward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

void turnLeft(unsigned int spd, unsigned int time_length) {
  turnLeftMotorBackward(spd);
  turnRightMotorForward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

void turnRight(unsigned int spd, unsigned int time_length) {
  turnLeftMotorForward(spd);
  turnRightMotorBackward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

void setup() {
  initialize();
}

void loop() {
  bool continueBot = true;
  float leftLight = 1000;
  float rightLight = 1000;

  // While not near the light, navigate around
  // obstacles and toward the light
  while (continueBot) {
    // Obstacle detection
    if( leftObstacle() && rightObstacle() ) {
      driveBack(SPEED, DRIVE_TIME);
      turnLeft(SPEED, TURN_TIME);
    }
    else if ( leftObstacle() ) {
      driveBack(SPEED, DRIVE_TIME);
      turnRight(SPEED, TURN_TIME);
    }
    else if ( rightObstacle() ) {
      driveBack(SPEED, DRIVE_TIME);
      turnLeft(SPEED, TURN_TIME);
    } // End of obstacle detection
    
    driveForw(SPEED, DRIVE_TIME);
    
    // Light detection
    if( !leftObstacle() && !rightObstacle() ) {
      leftLight = getLeftLight();
      rightLight = getRightLight();
      
      if ( (leftLight < LIGHT_READ_SINGLE) ||
           (rightLight < LIGHT_READ_SINGLE) ||
           ( (leftLight+rightLight) < LIGHT_READ_BOTH) ) {
        continueBot = false;
      }
      else {
        if ( (leftLight / rightLight) < LIGHT_RATIO_LEFT) {
          turnLeft(SPEED, TURN_TIME);
        }
        else if ( (leftLight / rightLight) > LIGHT_RATIO_RIGHT) {
          turnRight(SPEED, TURN_TIME);
        }
        else {
          driveForw(SPEED, TURN_TIME);
        }
      }
    } // End of light detection
    
  } // End of move bot toward light

  playSound(300, 1000);
  halt();
}
