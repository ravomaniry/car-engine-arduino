#include "glow_plug.h"
#include "communication.h"

// Glow plug configuration
const unsigned long GLOW_TIME_SECONDS = 10;
const int GLOW_PLUG_TRANSISTOR_PIN = 2; // D2: GREEN
const int GLOW_PLUG_BUTTON_PIN = 3; // D3: RED
const unsigned long GLOW_SWITCH_DEBOUNCE_MS = 100;

// Glow plug state variables
static int glowLastButtonState = HIGH; // The previous reading from the input pin
static unsigned long glowLastDebounceTime = 0; // The last time the output pin was toggled
static bool glowPlugIsActive = false;
static unsigned long glowEndTime = 0; // When the glow plug will be disabled
static bool buttonPressed = false; // Track if button is currently being pressed

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

    // Check for button press (transition from HIGH to LOW)
    if (buttonState == LOW && !buttonPressed) {
      buttonPressed = true; // Mark button as pressed
      unsigned long currentTime = millis();
      
      if (!glowPlugIsActive) {
        // Start glow plug if not active
        glowPlugIsActive = true;
        glowEndTime = currentTime + (GLOW_TIME_SECONDS * 1000);
        digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, HIGH); // Turn on the glow plug
        updateGlowState(true); // Update state and send data
        Serial.print("Glow plug activated! GLOW_TIME_SECONDS = ");
        Serial.println(GLOW_TIME_SECONDS);
      } else {
        // Add half the original glow time to the end time if already active
        unsigned long timeToAdd = (GLOW_TIME_SECONDS * 1000) / 2; // Add half of 10 seconds = 5 seconds
        glowEndTime += timeToAdd;
        int remainingSeconds = (glowEndTime - currentTime) / 1000;
        Serial.print("Glow time extended by ");
        Serial.print(timeToAdd / 1000);
        Serial.print("s! Remaining: ");
        Serial.print(remainingSeconds);
        Serial.println("s");
      }
    }
    
    // Check for button release (transition from LOW to HIGH)
    if (buttonState == HIGH && buttonPressed) {
      buttonPressed = false; // Mark button as released
    }
  }

  // If the glow plug is active, check if the end time has been reached
  if (glowPlugIsActive) {
    if (millis() >= glowEndTime) {
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

int getRemainingGlowTime() {
  if (!glowPlugIsActive) {
    return 0;
  }
  
  unsigned long currentTime = millis();
  if (currentTime >= glowEndTime) {
    return 0;
  }
  
  return (glowEndTime - currentTime) / 1000; // Return remaining seconds
}
