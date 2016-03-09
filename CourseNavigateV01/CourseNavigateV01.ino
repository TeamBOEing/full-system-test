/*
 * Course Navigate
 * Author: Cody Crossley, Corbin Murrow
 * Date: 20 January 2016
 * Version: 1.2
 * 
 * This program will be used to test a BOEbot's ability to navigate toward a light
 * and avoid obstacles.
 * 
 * ======= VERSION HISTORY =======
 * Version 1.2: Switched light checks back to lower readings = brighter light - CC - 20 January 2016
 * Version 1.1: Fixed light checks (higher is bright instead of lower) - CC & CM - 04 November 2015
 * Version 1.0: Created file and basic functionality - CC - 02 November 2015
 */

#include <BOEbot.h>
#include <Servo.h>

const unsigned int SPEED = 200;
const unsigned int DRIVE_TIME = 500;
const unsigned int TURN_TIME_LIGHT = 75;
const unsigned int TURN_TIME_AVOID = 250;
const unsigned int LIGHT_READ_SINGLE = 215;
const unsigned int LIGHT_READ_BOTH = LIGHT_READ_SINGLE * 1.85;
const float LIGHT_RATIO_LEFT = 1.1;
const float LIGHT_RATIO_RIGHT = 0.9;

const bool DEBUG_MOTORS = false;     // When true display motor info
const bool DEBUG_LIGHTS = false;     // When true display light info
const bool DEBUG_OBSTACLE = false;   // When true display obstacle info

// Drives BOEbot forward
void driveForw(unsigned int spd, unsigned int time_length) {
  if (DEBUG_MOTORS)
    Serial.println("Driving forward.");
  
  turnLeftMotorForward(spd);
  turnRightMotorForward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

// Drives BOEbot backward
void driveBack(unsigned int spd, unsigned int time_length) {
  if (DEBUG_MOTORS)
    Serial.println("Driving backward.");
  
  turnLeftMotorBackward(spd);
  turnRightMotorBackward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

// Turns BOEbot left
void turnLeft(unsigned int spd, unsigned int time_length) {
  if (DEBUG_MOTORS)
    Serial.println("Turning left.");
  
  turnLeftMotorBackward(spd);
  turnRightMotorForward(spd);
  delay(time_length);
  stopLeftMotor();
  stopRightMotor();
}

// Turns BOEbot right
void turnRight(unsigned int spd, unsigned int time_length) {
  if (DEBUG_MOTORS)
    Serial.println("Turning right.");
  
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
  bool continueBot = true;    // Run BOEbot until near light
  float leftLight;            // Store left light reading for comparison
  float rightLight;           // Store right light reading for comparison

  // While not near the light, navigate around obstacles and toward the light
  while (continueBot) {
    if (DEBUG_MOTORS || DEBUG_LIGHTS || DEBUG_OBSTACLE)
      Serial.println("\n ***** New Loop *****\n");
    
    // Obstacle detection
    if( leftObstacle() && rightObstacle() ) {
      if (DEBUG_OBSTACLE)
        Serial.println("Obstacle in front.");
      
      driveBack(SPEED, DRIVE_TIME);
      turnLeft(SPEED, TURN_TIME_AVOID);
    }
    else if ( leftObstacle() ) {
      if (DEBUG_OBSTACLE)
        Serial.println("Obstacle to left.");
      
      driveBack(SPEED, DRIVE_TIME);
      turnRight(SPEED, TURN_TIME_AVOID);
    }
    else if ( rightObstacle() ) {
      if (DEBUG_OBSTACLE)
        Serial.println("Obstacle to right.");
      
      driveBack(SPEED, DRIVE_TIME);
      turnLeft(SPEED, TURN_TIME_AVOID);
    } // End of obstacle detection
    
    driveForw(SPEED, DRIVE_TIME);   // Always move forward
    
    // Navigate toward light if no new obstacles in front of bot
    if( !leftObstacle() && !rightObstacle() ) {
      leftLight = getLeftLight() + 1;     // 0 readings possible, so +1
      rightLight = getRightLight() + 1;   // 0 readings possible, so +1

      if (DEBUG_LIGHTS) {
        Serial.print("Left Light: " + (String)leftLight);
        Serial.print("\tRight Light: " + (String)rightLight);
        Serial.println("\tL/R: " + (String)(leftLight/rightLight));
      }
      
      // If close to light indicate finished with course
      if ( (leftLight < LIGHT_READ_SINGLE) ||
           (rightLight < LIGHT_READ_SINGLE) ||
           ( (leftLight + rightLight) < LIGHT_READ_BOTH) ) {
        continueBot = false;

        if (DEBUG_LIGHTS)
          Serial.println("Arrived at light.");
      }
      // If not close to light, turn toward or move toward light
      else {
        // If not pointing toward light, turn toward without moving forward
        if ( (leftLight / rightLight) < LIGHT_RATIO_LEFT) {
          if (DEBUG_LIGHTS)
            Serial.println("Light to left.");
          
          turnLeft(SPEED, TURN_TIME_LIGHT);
        }
        else if ( (leftLight / rightLight) > LIGHT_RATIO_RIGHT) {
          if (DEBUG_LIGHTS)
            Serial.println("Light to right.");
            
          turnRight(SPEED, TURN_TIME_LIGHT);
        }
        // Facing light
        else {
          if (DEBUG_LIGHTS)
            Serial.println("Light in front.");
        }
      }
    } // End of light detection
  } // End of move bot toward light

  Serial.println("\nCourse Completed!\n");
  
  // Generic sound to indicate course completed
  playSound(300, 1000);
  halt();
}
