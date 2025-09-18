#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include <Ultrasonic.h>
#include <QTRSensors.h>

#define LEFT_EYE_TRIG_PIN       10
#define LEFT_EYE_ECHO_PIN       11
#define RIGHT_EYE_TRIG_PIN      12
#define RIGHT_EYE_ECHO_PIN      13
#define LEFT_NOSE_PIN           1
#define RIGHT_NOSE_PIN          2
#define NOSE_COUNT              2
#define MAX_REFLECTANCE         4096
#define MAX_DISTANCE            100
#define REFLECTANCE_THRESHOLD   2000
#define DISTANCE_THRESHOLD      50
#define ULTRASONIC_TIMEOUT      10000UL  // 10 ms

enum Position
{
  None, 
  Left, 
  Right, 
  Front, 
  Out
};

class SensorController {
private:
    Ultrasonic leftEye;
    Ultrasonic rightEye;
    QTRSensors qtr;
    uint16_t sensorValues[NOSE_COUNT];

    void refresh_boundary_position();
    void refresh_enemy_position();

    uint sample_distance(Ultrasonic& sensor);

public:
    SensorController();
    void init();
    void refresh();
};

extern SensorController sensors;

extern int16_t leftDistance;
extern int16_t rightDistance;

extern int16_t leftReflectance;
extern int16_t rightReflectance;

extern Position boundaryPosition;
extern Position prevBoundaryPosition;
extern Position enemyPosition;
extern Position prevEnemyPosition;

#endif
