#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

#define MOTOR_A_PIN_0 44
#define MOTOR_A_PIN_1 43
#define MOTOR_A_ENABLE_PIN 18
#define MOTOR_B_PIN_0 16
#define MOTOR_B_PIN_1 21
#define MOTOR_B_ENABLE_PIN 17

#define PWM_CHANNEL_A    0
#define PWM_CHANNEL_B    1
#define PWM_FREQ_HZ    5000
#define PWM_RES_BITS   8

#define NORMAL_SPEED 200
#define MAX_SPEED 255

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
    void motorA_forward(uint8_t speed = NORMAL_SPEED);
    void motorA_backward(uint8_t speed = NORMAL_SPEED);
    void motorA_stop();

    // Motor B control
    void motorB_forward(uint8_t speed = NORMAL_SPEED);
    void motorB_backward(uint8_t speed = NORMAL_SPEED);
    void motorB_stop();

    // Stop both motors
    void stop();

    // Move forward (both motors forward at speed)
    void forward(uint8_t speed = NORMAL_SPEED);

    // Move backward (both motors backward at speed)
    void backward(uint8_t speed = NORMAL_SPEED);

    // Turn left (motor A backward, motor B forward)
    void turnLeft(uint8_t speed = NORMAL_SPEED);

    // Turn right (motor B backward, motor A forward)
    void turnRight(uint8_t speed = NORMAL_SPEED);

private:
    uint8_t _motorA_in1, _motorA_in2, _motorA_enable;
    uint8_t _motorB_in1, _motorB_in2, _motorB_enable;

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
