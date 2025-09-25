#include <Arduino.h>
#include "sensor_controller.h"
#include "motor_controller.h"
#include "display_controller.h"

SensorController sensors;
MotorController motors;
DisplayController display;

void search_mode()
{
  int pattern = (millis() / (1000 * 2)) % 4; // Change pattern every 2 seconds
  if (pattern == 0) {
    motors.turnLeft();
  } else if (pattern == 1) {
    motors.forward();
  } else if (pattern == 2) {
    motors.turnRight();
  } else {
    motors.forward();
  }
}

void destroy_mode()
{
  if (enemyPosition == Front) { // enemy in front
    motors.forward(MAX_SPEED);
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
  // Enable JST connector.
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  display.init();
  sensors.init();
  motors.init();
}

void loop() {
  sensors.refresh();

  // Check if we're out of bounds.
  if (boundaryPosition != None)
  {
    display.set_text("Out of bounds!");
    return_to_safety();
  }
  else
  {
    // Make a decision.
    if (enemyPosition == None) { // no enemy position
      display.set_text("Searching...");
      search_mode();
    } else 
    {
      display.set_text("DESTROYING");
      destroy_mode();
    }
  }

  // Refresh the display info.
  display.refresh();
}
