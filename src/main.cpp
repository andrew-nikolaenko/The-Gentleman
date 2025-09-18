#include <Arduino.h>
#include "sensor_controller.h"
#include "motor_controller.h"
#include "display_controller.h"

SensorController sensors;
MotorController motors;
DisplayController display;

void search_mode()
{
  if (prevEnemyPosition == Left || None) { // left or none
    motors.turnLeft();
  } else if (prevEnemyPosition == Right) { // right
    motors.turnRight();
  }
}

void destroy_mode()
{
  if (enemyPosition == Front) { // enemy in front
    motors.forward();
  } else if (enemyPosition == Left) { // enemy to the left
    motors.turnLeft();
  } else if (enemyPosition == Right) { // enemy to the right
    motors.turnRight();
  }
}

void return_to_safety() {
  if (boundaryPosition == Front) {
    motors.turnLeft();
  } else if (boundaryPosition == Left) {
    motors.turnRight();
  } else if (boundaryPosition == Right) {
    motors.turnLeft();
  }
}

void setup() {
  Serial.begin(9600);
  display.init();
  sensors.init();
  motors.init();
}

void loop() {
  sensors.refresh();

  // Check if we're out of bounds.
  if (boundaryPosition != None)
  {
    return_to_safety();
  }
  else
  {
    // Make a decision.
    if (enemyPosition == None) { // no enemy position
      search_mode();
    } else 
    {
      destroy_mode();
    }
  }

  // Refresh the display info.
  display.refresh();
}
