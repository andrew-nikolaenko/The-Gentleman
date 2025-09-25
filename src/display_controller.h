#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include <TFT_eSPI.h>
#include "sensor_controller.h"
#include "motor_controller.h"

class DisplayController {
private:
    TFT_eSPI tft;
    
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

    bool textChanged;
    String text;

    void display_boundary_info();
    void display_enemy_info();
    void lookLeft();
    void lookRight();
    void lookCenter();
    void drawEyes(int xOffset);
    void display_robot_direction();
    void draw_meter(int x, int y, int width, int height, int percentage, uint32_t color, bool vertical, int thresholdPercent, bool reverse);
    void display_text();

public:
    DisplayController();
    void init();
    void refresh();
    void set_text(const String& newText);
};

extern DisplayController display;

#endif
