#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

#define MOTOR_A_PIN_0 43
#define MOTOR_A_PIN_1 44
#define MOTOR_B_PIN_0 21
#define MOTOR_B_PIN_1 16

enum Direction
{
    GoingLeft, 
    GoingRight, 
    GoingForwards,
    GoingBackwards
};

class MotorController {
public:
    MotorController();

    // Initialize pins (call in setup)
    void init();

    // Motor A control
    void motorA_forward(uint8_t speed = 255);
    void motorA_backward(uint8_t speed = 255);
    void motorA_stop();

    // Motor B control
    void motorB_forward(uint8_t speed = 255);
    void motorB_backward(uint8_t speed = 255);
    void motorB_stop();

    // Stop both motors
    void stop();

    // Move forward (both motors forward at speed)
    void forward(uint8_t speed = 255);

    // Move backward (both motors backward at speed)
    void backward(uint8_t speed = 255);

    // Turn left (motor A slower or backward, motor B forward)
    void turnLeft(uint8_t speed = 255);

    // Turn right (motor B slower or backward, motor A forward)
    void turnRight(uint8_t speed = 255);

private:
    uint8_t _motorA_in1, _motorA_in2;
    uint8_t _motorB_in1, _motorB_in2;

    // Helper to set motor pins with speed and direction
    void setMotor(uint8_t in1Pin, uint8_t in2Pin, int16_t speed, bool isLeft);
};

extern MotorController motors;

extern int16_t motorA1;
extern int16_t motorA2;
extern int16_t motorB1;
extern int16_t motorB2;

extern Direction prevRobotDirection;
extern Direction robotDirection;

#endif
