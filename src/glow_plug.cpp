#include "glow_plug.h"
#include "communication.h"

// Glow plug configuration
const unsigned long GLOW_TIME_SECONDS = 5;
const int GLOW_PLUG_TRANSISTOR_PIN = 2; // D2: GREEN
const int GLOW_PLUG_BUTTON_PIN = 3; // D3: RED
const unsigned long GLOW_SWITCH_DEBOUNCE_MS = 100;

// Glow plug state variables
static int glowLastButtonState = HIGH; // The previous reading from the input pin
static unsigned long glowLastDebounceTime = 0; // The last time the output pin was toggled
static bool glowPlugIsActive = false;
static unsigned long glowStartTime = 0;

void setupGlowPlug() {
  // Set glow plug pin as an output
  pinMode(GLOW_PLUG_TRANSISTOR_PIN, OUTPUT);
  digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, LOW); // Ensure glow plug is off initially

  // Set glow plug button pin as an input with internal pull-up resistor
  pinMode(GLOW_PLUG_BUTTON_PIN, INPUT_PULLUP);
}

void handleGlowPlug() {
  // Read the state of the glow plug button
  int buttonState = digitalRead(GLOW_PLUG_BUTTON_PIN);

  // If the switch changed, due to noise or pressing:
  if (buttonState != glowLastButtonState) {
    // reset the debouncing timer
    glowLastDebounceTime = millis();
  }

  if ((millis() - glowLastDebounceTime) > GLOW_SWITCH_DEBOUNCE_MS) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // Check if the button is pressed (LOW) and the glow plug is not currently active
    if (buttonState == LOW && !glowPlugIsActive) {
      glowPlugIsActive = true;
      glowStartTime = millis(); // Record the start time
      digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, HIGH); // Turn on the glow plug
      updateGlowState(true); // Update state and send data
      Serial.println("Glow plug activated!");
    }
  }

  // If the glow plug is active, check if the glow time has elapsed
  if (glowPlugIsActive) {
    if (millis() - glowStartTime >= GLOW_TIME_SECONDS * 1000) { // Convert seconds to milliseconds
      glowPlugIsActive = false;
      digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, LOW); // Turn off the glow plug
      updateGlowState(false); // Update state and send data
      Serial.println("Glow plug deactivated (time elapsed).");
    }
  }

  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  glowLastButtonState = buttonState;
}

bool isGlowPlugActive() {
  return glowPlugIsActive;
}
