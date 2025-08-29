#include <Arduino.h>

// Glow plug settings
const unsigned long GLOW_TIME_SECONDS = 5; // Glow time in seconds
const int GLOW_PLUG_PIN = 2;       // Arduino pin connected to the glow plug MOSFET (D2)
const int GLOW_PLUG_BUTTON_PIN = 3; // Arduino pin connected to the glow plug button (D3)
const unsigned long DEBOUNCE_DELAY = 100; // Debounce time in milliseconds

// State variables
bool isGlowPlugActive = false;
unsigned long glowStartTime = 0;

// Debounce variables
int lastButtonState = HIGH; // The previous reading from the input pin
unsigned long lastDebounceTime = 0; // The last time the output pin was toggled

// Function to handle glow plug logic
void handleGlowPlug() {
  // Read the state of the glow plug button
  int reading = digitalRead(GLOW_PLUG_BUTTON_PIN);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // Check if the button is pressed (LOW) and the glow plug is not currently active
    if (reading == LOW && !isGlowPlugActive) {
      isGlowPlugActive = true;
      glowStartTime = millis(); // Record the start time
      digitalWrite(GLOW_PLUG_PIN, HIGH); // Turn on the glow plug
      Serial.println("Glow plug activated!");
    }
  }

  // If the glow plug is active, check if the glow time has elapsed
  if (isGlowPlugActive) {
    if (millis() - glowStartTime >= GLOW_TIME_SECONDS * 1000) { // Convert seconds to milliseconds
      isGlowPlugActive = false;
      digitalWrite(GLOW_PLUG_PIN, LOW); // Turn off the glow plug
      Serial.println("Glow plug deactivated (time elapsed).");
    }
  }

  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

// Function to set up glow plug related pins
void setupGlowPlug() {
  // Set glow plug pin as an output
  pinMode(GLOW_PLUG_PIN, OUTPUT);
  digitalWrite(GLOW_PLUG_PIN, LOW); // Ensure glow plug is off initially

  // Set glow plug button pin as an input with internal pull-up resistor
  pinMode(GLOW_PLUG_BUTTON_PIN, INPUT_PULLUP);
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Engine Control Unit - Starting up...");

  setupGlowPlug(); // Call the glow plug setup function

  Serial.println("Setup complete. Waiting for button press.");
}

void loop() {
  handleGlowPlug(); // Call the glow plug handling function
}
