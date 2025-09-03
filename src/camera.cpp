#include "camera.h"
#include "communication.h"

// Camera configuration
const int CAMERA_MOSFET_PIN = 6; // D6: Camera 12V MOSFET control
const int CAMERA_BUTTON_PIN = 7; // D7: Manual camera activation button
const unsigned long CAMERA_BUTTON_DEBOUNCE_MS = 100;
const unsigned long CAMERA_AUTO_OFF_TIMEOUT_MS = 60000; // 1 minute (60 seconds)
const unsigned long CAMERA_MANUAL_TIMEOUT_MS = 15000;   // 15 seconds

// Camera state tracking
static bool cameraIsActive = false;
static bool cameraActivatedByReverse = false;
static bool cameraActivatedByButton = false;
static unsigned long cameraStartTime = 0;
static int cameraLastButtonState = HIGH;
static unsigned long cameraLastDebounceTime = 0;

void setupCamera() {
  // Set camera MOSFET pin as output
  pinMode(CAMERA_MOSFET_PIN, OUTPUT);
  digitalWrite(CAMERA_MOSFET_PIN, LOW); // Ensure camera is off initially

  // Set camera button pin as input with internal pull-up resistor
  pinMode(CAMERA_BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize camera state
  updateCameraState(false);
}

void handleCamera() {
  // Handle manual camera button
  int buttonState = digitalRead(CAMERA_BUTTON_PIN);

  // If the switch changed, due to noise or pressing:
  if (buttonState != cameraLastButtonState) {
    // reset the debouncing timer
    cameraLastDebounceTime = millis();
  }

  if ((millis() - cameraLastDebounceTime) > CAMERA_BUTTON_DEBOUNCE_MS) {
    // Check if the button is pressed (LOW) and camera is not currently active
    if (buttonState == LOW && !cameraIsActive) {
      cameraIsActive = true;
      cameraActivatedByButton = true;
      cameraActivatedByReverse = false;
      cameraStartTime = millis();
      digitalWrite(CAMERA_MOSFET_PIN, HIGH);
      updateCameraState(true);
      Serial.println("Camera activated by manual button!");
    }
  }

  // Handle camera timeout logic
  if (cameraIsActive) {
    unsigned long elapsedTime = millis() - cameraStartTime;
    bool shouldTurnOff = false;

    if (cameraActivatedByButton && elapsedTime >= CAMERA_MANUAL_TIMEOUT_MS) {
      // Manual activation timeout (15 seconds)
      shouldTurnOff = true;
      Serial.println("Camera turned off - manual timeout (15 seconds)");
    } else if (!cameraActivatedByReverse && !cameraActivatedByButton && elapsedTime >= CAMERA_AUTO_OFF_TIMEOUT_MS) {
      // Auto-off timeout (1 minute) - only if not activated by reverse or button
      shouldTurnOff = true;
      Serial.println("Camera turned off - auto timeout (1 minute)");
    }

    if (shouldTurnOff) {
      cameraIsActive = false;
      cameraActivatedByButton = false;
      cameraActivatedByReverse = false;
      digitalWrite(CAMERA_MOSFET_PIN, LOW);
      updateCameraState(false);
    }
  }

  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  cameraLastButtonState = buttonState;
}

void activateCameraByReverse() {
  if (!cameraIsActive) {
    // Reverse gear engaged - activate camera
    cameraIsActive = true;
    cameraActivatedByReverse = true;
    cameraActivatedByButton = false;
    cameraStartTime = millis();
    digitalWrite(CAMERA_MOSFET_PIN, HIGH);
    updateCameraState(true);
    Serial.println("Camera activated by reverse gear!");
  }
}

void deactivateCameraByReverse() {
  if (cameraActivatedByReverse) {
    // Reverse gear disengaged - start 1 minute countdown to turn off camera
    cameraActivatedByReverse = false;
    cameraStartTime = millis(); // Reset timer for auto-off
    Serial.println("Reverse gear disengaged - camera will turn off in 1 minute");
  }
}

bool isCameraActive() {
  return cameraIsActive;
}
