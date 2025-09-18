#ifndef EYES_H
#define EYES_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class Eyes {
private:
    TFT_eSPI& tft;
    
    // Constants for eye dimensions
    static const int EYE_RADIUS = 50;
    static const int PUPIL_RADIUS = 20;
    static const int PUPIL_OFFSET_Y = 25;    // Vertical offset
    static const int PUPIL_OFFSET_X = 25;    // Horizontal offset for looking left/right
    
    // Eye positions
    int leftEyeX;
    int rightEyeX;
    int eyeY;
    int currentEyeState;  // 0: center, -1: left, 1: right

    // Button pins
    static const int BTN_LEFT = 0;   // Left button
    static const int BTN_RIGHT = 14; // Right button

public:
    Eyes(TFT_eSPI& tftDisplay);
    void init();
    void drawEyes(int xOffset);
    void update();
    void lookLeft();
    void lookRight();
    void lookCenter();
};

extern Eyes eyes;

#endif