#include "eyes.h"

Eyes eyes(tft);

Eyes::Eyes(TFT_eSPI& tftDisplay) : tft(tftDisplay), currentEyeState(0) {}

void Eyes::init() {
    // Initialize buttons
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    
    // Calculate eye positions
    leftEyeX = tft.width()/3;
    rightEyeX = 2*tft.width()/3;
    eyeY = tft.height()/3;
    
    // Draw initial eyes looking forward
    drawEyes(0);
}

void Eyes::drawEyes(int xOffset) {
  // Clear previous eyes
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(0, 0, tft.width(), tft.height(), TFT_WHITE);
  
  // Draw the white circles (eyes)
  tft.fillCircle(leftEyeX, eyeY, EYE_RADIUS, TFT_WHITE);
  tft.fillCircle(rightEyeX, eyeY, EYE_RADIUS, TFT_WHITE);
  
  // Draw the black pupils with x offset
  tft.fillCircle(leftEyeX + xOffset, eyeY - PUPIL_OFFSET_Y, PUPIL_RADIUS, TFT_BLACK);
  tft.fillCircle(rightEyeX + xOffset, eyeY - PUPIL_OFFSET_Y, PUPIL_RADIUS, TFT_BLACK);
  
  // Set up text parameters
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  
  // Draw different text based on eye direction
  if (xOffset < 0) {
    tft.drawString("Rotating Left", tft.width()/2, tft.height()*4/5);
  } else if (xOffset > 0) {
    tft.drawString("Rotating Right", tft.width()/2, tft.height()*4/5);
  } else {
    tft.drawString("Charging Ahead!", tft.width()/2, tft.height()*4/5);
  }
}

void Eyes::lookLeft() {
    if (currentEyeState != -1) {
        drawEyes(-PUPIL_OFFSET_X);
        currentEyeState = -1;
    }
}

void Eyes::lookRight() {
    if (currentEyeState != 1) {
        drawEyes(PUPIL_OFFSET_X);
        currentEyeState = 1;
    }
}

void Eyes::lookCenter() {
    if (currentEyeState != 0) {
        drawEyes(0);
        currentEyeState = 0;
    }
}

void Eyes::update() {
    int newEyeState = 0;
    
    // Determine new eye state based on buttons
    if (digitalRead(BTN_LEFT) == LOW) {
        newEyeState = -1;  // Look left
    }
    else if (digitalRead(BTN_RIGHT) == LOW) {
        newEyeState = 1;   // Look right
    }
    
    // Only redraw if the eye state has changed
    if (newEyeState != currentEyeState) {
        switch(newEyeState) {
            case -1:
                lookLeft();
                break;
            case 1:
                lookRight();
                break;
            default:
                lookCenter();
                break;
        }
        delay(50);  // Small debounce delay after state change
    }
    
    delay(10);  // Small delay to prevent CPU hogging
}
