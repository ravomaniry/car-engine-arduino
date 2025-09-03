#include "reverse_gear.h"
#include "communication.h"
#include "camera.h"

// Reverse gear configuration
const byte REVERSE_GEAR_PIN = 5; // D5: Reverse gear switch input
const unsigned long REVERSE_GEAR_DEBOUNCE_MS = 100;

// Reverse gear state variables
static bool reverseGearEngaged = false;
static uint8_t reverseLastRawReading = HIGH;
static bool reverseStableState = HIGH;          // debounced state
static unsigned long reverseLastChangeMillis = 0;

void setupReverseGear() {
  pinMode(REVERSE_GEAR_PIN, INPUT_PULLUP);  // use internal pull-up

  // read initial state
  reverseLastRawReading = digitalRead(REVERSE_GEAR_PIN);
  reverseStableState = reverseLastRawReading;
  // For reverse gear switch: LOW = reverse engaged, HIGH = not in reverse
  reverseGearEngaged = (reverseStableState == LOW);
  updateReverseGearState(reverseGearEngaged); // Initialize reverse gear state

  reverseLastChangeMillis = millis();
}

void handleReverseGear() {
  uint8_t raw = digitalRead(REVERSE_GEAR_PIN);

  // if input changed, reset timer
  if (raw != reverseLastRawReading) {
    reverseLastChangeMillis = millis();
    reverseLastRawReading = raw;
  } else {
    // if stable long enough and different from stable state → update
    if (raw != reverseStableState && (millis() - reverseLastChangeMillis >= REVERSE_GEAR_DEBOUNCE_MS)) {
      reverseStableState = raw;
      // For reverse gear switch: LOW = reverse engaged, HIGH = not in reverse
      reverseGearEngaged = (reverseStableState == LOW);

      // Update sensor state
      updateReverseGearState(reverseGearEngaged);

      // Handle camera activation based on reverse gear
      if (reverseGearEngaged) {
        activateCameraByReverse();
      } else {
        deactivateCameraByReverse();
      }
    }
  }
}

bool isReverseGearEngaged() {
  return reverseGearEngaged;
}
