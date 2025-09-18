#include "sensor_controller.h"

int16_t leftDistance;
int16_t rightDistance;

int16_t leftReflectance;
int16_t rightReflectance;

Position boundaryPosition;
Position prevBoundaryPosition;
Position enemyPosition;
Position prevEnemyPosition;

SensorController::SensorController() 
    : leftEye(LEFT_EYE_TRIG_PIN, LEFT_EYE_ECHO_PIN),
      rightEye(RIGHT_EYE_TRIG_PIN, RIGHT_EYE_ECHO_PIN),
      qtr() {
}

void SensorController::init() {
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){LEFT_NOSE_PIN, RIGHT_NOSE_PIN}, NOSE_COUNT);
    
    boundaryPosition = None;
    prevBoundaryPosition = None;
    enemyPosition = None;
    prevEnemyPosition = None;
}

void SensorController::refresh_boundary_position() {
    prevBoundaryPosition = boundaryPosition;

    qtr.read(sensorValues);

    leftReflectance = sensorValues[0];
    rightReflectance = sensorValues[1];
    
    if (leftReflectance < REFLECTANCE_THRESHOLD && rightReflectance < REFLECTANCE_THRESHOLD) {
        boundaryPosition = Front;
    } else if (leftReflectance < REFLECTANCE_THRESHOLD) {
        boundaryPosition = Left;
    } else if (rightReflectance < REFLECTANCE_THRESHOLD) {
        boundaryPosition = Right;
    } else {
        boundaryPosition = None;
    }
}

void SensorController::refresh_enemy_position() {
    prevEnemyPosition = enemyPosition;
    
    leftDistance = leftEye.read();
    rightDistance = rightEye.read();
    
    if (leftDistance < DISTANCE_THRESHOLD && rightDistance < DISTANCE_THRESHOLD) {
        enemyPosition = Front;
    } else if (leftDistance < DISTANCE_THRESHOLD) {
        enemyPosition = Left;
    } else if (rightDistance < DISTANCE_THRESHOLD) {
        enemyPosition = Right;
    } else {
        enemyPosition = None;
    }
}

void SensorController::refresh() {
    refresh_boundary_position();
    refresh_enemy_position();
}