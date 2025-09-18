#include <Arduino.h>
#include <TFT_eSPI.h>

#define BTN_LEFT 0   // Left button
#define BTN_RIGHT 14 // Right button

// Constants for eye dimensions
const int EYE_RADIUS = 50;
const int PUPIL_RADIUS = 20;
const int PUPIL_OFFSET_Y = 25;    // Vertical offset
const int PUPIL_OFFSET_X = 25;    // Horizontal offset for looking left/right

TFT_eSPI tft = TFT_eSPI();

// Global variables for eye positions
int leftEyeX, rightEyeX, eyeY;

// Function to draw eyes with pupils at specified offset
void drawEyes(int xOffset) {
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

void setup() {
  Serial.begin(115200);
  
  // Initialize buttons
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  
  // Initialize display
  tft.init();
  tft.setRotation(1);
  
  // Calculate eye positions (now global variables)
  leftEyeX = tft.width()/3;
  rightEyeX = 2*tft.width()/3;
  eyeY = tft.height()/3;
  
  // Draw initial eyes looking forward
  drawEyes(0);
  
  Serial.println("Display initialized");
}

// Keep track of current eye state
int currentEyeState = 0;  // 0: center, -1: left, 1: right

void loop() {
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
        drawEyes(-PUPIL_OFFSET_X);  // Look left
        break;
      case 1:
        drawEyes(PUPIL_OFFSET_X);   // Look right
        break;
      default:
        drawEyes(0);                // Look center
        break;
    }
    currentEyeState = newEyeState;
    delay(50);  // Small debounce delay after state change
  }
  
  delay(10);  // Small delay to prevent CPU hogging
}
