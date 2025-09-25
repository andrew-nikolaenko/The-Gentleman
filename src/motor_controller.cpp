#include "motor_controller.h"

int16_t motorA1;
int16_t motorA2;
int16_t motorB1;
int16_t motorB2;

Direction prevRobotDirection;
Direction robotDirection;

MotorController::MotorController()
    : _motorA_in1(MOTOR_A_PIN_0), _motorA_in2(MOTOR_A_PIN_1), _motorA_enable(MOTOR_A_ENABLE_PIN),
      _motorB_in1(MOTOR_B_PIN_0), _motorB_in2(MOTOR_B_PIN_1), _motorB_enable(MOTOR_B_ENABLE_PIN) {}

void MotorController::init() {
    // Set direction pins as outputs
    pinMode(_motorA_in1, OUTPUT);
    pinMode(_motorA_in2, OUTPUT);
    pinMode(_motorB_in1, OUTPUT);
    pinMode(_motorB_in2, OUTPUT);
    
    // Initialize direction pins to LOW
    digitalWrite(_motorA_in1, LOW);
    digitalWrite(_motorA_in2, LOW);
    digitalWrite(_motorB_in1, LOW);
    digitalWrite(_motorB_in2, LOW);

    // Setup PWM for dual motor speed control
    ledcSetup(PWM_CHANNEL_A, PWM_FREQ_HZ, PWM_RES_BITS);
    ledcSetup(PWM_CHANNEL_B, PWM_FREQ_HZ, PWM_RES_BITS);
    ledcAttachPin(_motorA_enable, PWM_CHANNEL_A);
    ledcAttachPin(_motorB_enable, PWM_CHANNEL_B);

    stop();

    prevRobotDirection = GoingLeft;
    robotDirection = GoingLeft;
}

// speed >0 forward, <0 backward, 0 stop
void MotorController::setMotor(uint8_t in1Pin, uint8_t in2Pin, int16_t speed, bool isLeft) {
    uint8_t pwmChannel = isLeft ? PWM_CHANNEL_A : PWM_CHANNEL_B;
    
    if (speed > 0) {
        // Forward
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
        ledcWrite(pwmChannel, speed);
        if (isLeft) {
            motorA1 = speed;
            motorA2 = 0;
        } else {
            motorB1 = speed;
            motorB2 = 0;
        }
    } else if (speed < 0) {
        // Backward
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
        ledcWrite(pwmChannel, -speed);
        if (isLeft) {
            motorA1 = 0;
            motorA2 = -speed;
        } else {
            motorB1 = 0;
            motorB2 = -speed;
        }
    } else {
        // Stop
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);
        ledcWrite(pwmChannel, 0);
        if (isLeft) {
            motorA1 = 0;
            motorA2 = 0;
        } else {
            motorB1 = 0;
            motorB2 = 0;
        }
    }
}

// Motor A controls
void MotorController::motorA_forward(uint8_t speed) {
    setMotor(_motorA_in1, _motorA_in2, speed, true);
}

void MotorController::motorA_backward(uint8_t speed) {
    setMotor(_motorA_in1, _motorA_in2, -speed, true);
}

void MotorController::motorA_stop() {
    setMotor(_motorA_in1, _motorA_in2, 0, true);
}

// Motor B controls
void MotorController::motorB_forward(uint8_t speed) {
    setMotor(_motorB_in1, _motorB_in2, speed, false);
}

void MotorController::motorB_backward(uint8_t speed) {
    setMotor(_motorB_in1, _motorB_in2, -speed, false);
}

void MotorController::motorB_stop() {
    setMotor(_motorB_in1, _motorB_in2, 0, false);
}

void MotorController::stop() {
    motorA_stop();
    motorB_stop();
}

void MotorController::forward(uint8_t speed) {
    motorA_forward(speed);
    motorB_forward(speed);

    prevRobotDirection = robotDirection;
    robotDirection = GoingForwards;
}

void MotorController::backward(uint8_t speed) {
    motorA_backward(speed);
    motorB_backward(speed);

    prevRobotDirection = robotDirection;
    robotDirection = GoingBackwards;
}

void MotorController::turnLeft(uint8_t speed) {
    motorA_backward(speed);
    motorB_forward(speed);

    prevRobotDirection = robotDirection;
    robotDirection = GoingLeft;
}

void MotorController::turnRight(uint8_t speed) {
    motorA_forward(speed);
    motorB_backward(speed);

    prevRobotDirection = robotDirection;
    robotDirection = GoingRight;
}
