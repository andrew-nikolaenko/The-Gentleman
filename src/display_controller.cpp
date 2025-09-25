#include "display_controller.h"

DisplayController::DisplayController() 
    : tft(TFT_eSPI()), currentEyeState(0) {}

void DisplayController::init() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    
    // Calculate eye positions
    leftEyeX = tft.width()/3;
    rightEyeX = 2*tft.width()/3;
    eyeY = tft.height()/2;

    textChanged = false;
    text = "";
    
    // Draw initial eyes looking forward
    drawEyes(0);
}

void DisplayController::refresh() {
    display_boundary_info();
    display_enemy_info();
    display_robot_direction();
    display_text();
}

void DisplayController::draw_meter(int x, int y, int width, int height, int percentage, uint32_t color, bool vertical, int thresholdPercent, bool reverse) {
    // Calculate filled area based on percentage
    int filledWidth = (percentage * width) / 100;
    int filledHeight = (percentage * height) / 100;
    
    // Draw the filled portion of the meter
    if (vertical) {
        int fillY = reverse ? y : (y + height - filledHeight);
        tft.fillRect(x + 1, fillY, width - 2, filledHeight, color);
    } else {
        int fillX = reverse ? (x + width - filledWidth) : x;
        tft.fillRect(fillX, y + 1, filledWidth, height - 2, color);
    }

    // Draw the unfilled portion of the meter
    if (vertical) {
        int unfilledHeight = height - filledHeight - 2;
        int unfillY = reverse ? (y + filledHeight + 1) : y;
        tft.fillRect(x + 1, unfillY, width - 2, unfilledHeight, TFT_BLACK);
    } else {
        int unfilledWidth = width - filledWidth - 2;
        int unfillX = reverse ? x : (x + filledWidth + 1);
        tft.fillRect(unfillX, y + 1, unfilledWidth, height - 2, TFT_BLACK);
    }

    // Draw threshold line if applicable
    if (thresholdPercent > 0 && thresholdPercent < 100) {
        if (vertical) {
            int thresholdY = y + (reverse ? (thresholdPercent * height / 100) : (height - (thresholdPercent * height / 100)));
            tft.drawLine(x, thresholdY, x + width, thresholdY, TFT_WHITE);
        } else {
            int thresholdX = x + (reverse ? (width - (thresholdPercent * width / 100)) : (thresholdPercent * width / 100));
            tft.drawLine(thresholdX, y, thresholdX, y + height, TFT_WHITE);
        }
    }
    
    // Draw the meter outline last
    tft.drawRect(x, y, width, height, TFT_WHITE);
}

void DisplayController::display_boundary_info() {
    int reflectanceThresholdPercent = 100 - ((float)REFLECTANCE_THRESHOLD / (float)MAX_REFLECTANCE) * 100;
    
    const int METER_WIDTH = 20;  // Width of the boundary meters
    const int MARGIN = 5;        // Space from the edge of the screen
    
    // Draw left boundary meter
    int leftPercentage = 100 - ((float)leftReflectance / (float)MAX_REFLECTANCE) * 100;
    draw_meter(MARGIN, 0, METER_WIDTH, tft.height(), leftPercentage, TFT_RED, true, reflectanceThresholdPercent, false);

    // Draw right boundary meter
    int rightPercentage = 100 - ((float)rightReflectance / (float)MAX_REFLECTANCE) * 100;
    draw_meter(tft.width() - METER_WIDTH - MARGIN, 0, METER_WIDTH, tft.height(), rightPercentage, TFT_RED, true, reflectanceThresholdPercent, false);
}

void DisplayController::display_enemy_info() {
    int distanceThresholdPercent = 100 - ((float)DISTANCE_THRESHOLD / (float)MAX_DISTANCE) * 100;

    const int METER_HEIGHT = 20;     // Height of the enemy meters
    const int TOP_MARGIN = 5;        // Space from the top of the screen
    const int SIDE_MARGIN = 30;      // Space from the sides to accommodate boundary meters
    
    // Calculate the width for each meter (half screen width minus margins)
    int meterWidth = (tft.width() - 2 * SIDE_MARGIN) / 2;

    // Draw left enemy proximity meter at the top left
    int leftPercentage = 100 - ((float)leftDistance / (float)MAX_DISTANCE) * 100;
    draw_meter(SIDE_MARGIN, TOP_MARGIN, meterWidth, METER_HEIGHT, leftPercentage, TFT_BLUE, false, distanceThresholdPercent, false);

    // Draw right enemy proximity meter at the top right
    int rightPercentage = 100 - ((float)rightDistance / (float)MAX_DISTANCE) * 100;
    draw_meter(SIDE_MARGIN + meterWidth, TOP_MARGIN, meterWidth, METER_HEIGHT, rightPercentage, TFT_BLUE, false, distanceThresholdPercent, true);
}

void DisplayController::drawEyes(int xOffset) {
    // Draw the white circles (eyes)
    tft.fillCircle(leftEyeX, eyeY, EYE_RADIUS, TFT_WHITE);
    tft.fillCircle(rightEyeX, eyeY, EYE_RADIUS, TFT_WHITE);
    
    // Draw the black pupils with x offset
    tft.fillCircle(leftEyeX + xOffset, eyeY - PUPIL_OFFSET_Y, PUPIL_RADIUS, TFT_BLACK);
    tft.fillCircle(rightEyeX + xOffset, eyeY - PUPIL_OFFSET_Y, PUPIL_RADIUS, TFT_BLACK);
}

void DisplayController::lookLeft() {
    drawEyes(-PUPIL_OFFSET_X);
}

void DisplayController::lookRight() {
    drawEyes(PUPIL_OFFSET_X);
}

void DisplayController::lookCenter() {
    drawEyes(0);
}

void DisplayController::display_robot_direction() {
    // Only redraw if the robot direction state has changed
    if (robotDirection != prevRobotDirection) {
        switch(robotDirection) {
            case GoingLeft:
                lookLeft();
                break;
            case GoingRight:
                lookRight();
                break;
            default:
                lookCenter();
                break;
        }
    }
}

void DisplayController::display_text() {
    // Only redraw if the text has changed
    if (textChanged) {
        tft.fillRect(30, tft.height() - 30, tft.width() - 60, 30, TFT_BLACK); // Clear previous text area
        tft.setCursor(35, tft.height() - 25);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.print(text);
        textChanged = false;
    }
}

void DisplayController::set_text(const String& newText) {
    if (text != newText) {
        text = newText;
        textChanged = true;
    }
}
