#include <Arduino.h>

// Glow plug
const unsigned long GLOW_TIME_SECONDS = 5;
const int GLOW_PLUG_TRANSISTOR_PIN = 2; // D2: GREEN
const int GLOW_PLUG_BUTTON_PIN = 3; // D3: RED
const unsigned long GLOW_SWITCH_DEBOUNCE_MS = 100;
int glowLastButtonState = HIGH; // The previous reading from the input pin
unsigned long glowLastDebounceTime = 0; // The last time the output pin was toggled
bool glowPlugIsActive = false;
unsigned long glowStartTime = 0;

// OIL pressure swich
const byte OIL_SWITCH_PIN = 4; // D4: BLUE
const byte BUILTIN_LED_PIN = 13;
const unsigned long OIL_DEBOUNCE_MS = 100;
bool oilIsLow = false;
uint8_t oilLastRawReading = HIGH;
bool oilStableState = HIGH;          // debounced state
unsigned long oilLastChangeMillis = 0;

// Function to handle glow plug logic
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
      Serial.println("Glow plug activated!");
    }
  }

  // If the glow plug is active, check if the glow time has elapsed
  if (glowPlugIsActive) {
    if (millis() - glowStartTime >= GLOW_TIME_SECONDS * 1000) { // Convert seconds to milliseconds
      glowPlugIsActive = false;
      digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, LOW); // Turn off the glow plug
      Serial.println("Glow plug deactivated (time elapsed).");
    }
  }

  // Save the reading. Next time through the loop, it'll be the lastButtonState:
  glowLastButtonState = buttonState;
}

void setupOilPressure() {
  pinMode(OIL_SWITCH_PIN, INPUT_PULLUP);  // use internal pull-up
  pinMode(BUILTIN_LED_PIN, OUTPUT);

  // read initial state
  oilLastRawReading = digitalRead(OIL_SWITCH_PIN);
  oilStableState = oilLastRawReading;
  oilIsLow = (oilStableState == LOW);
  digitalWrite(BUILTIN_LED_PIN, oilIsLow ? HIGH : LOW);

  oilLastChangeMillis = millis();
}

void handleOilPressure() {
  uint8_t raw = digitalRead(OIL_SWITCH_PIN);

  // if input changed, reset timer
  if (raw != oilLastRawReading) {
    oilLastChangeMillis = millis();
    oilLastRawReading = raw;
  } else {
    // if stable long enough and different from stable state → update
    if (raw != oilStableState && (millis() - oilLastChangeMillis >= OIL_DEBOUNCE_MS)) {
      oilStableState = raw;
      oilIsLow = (oilStableState == LOW);

      // update LED
      digitalWrite(BUILTIN_LED_PIN, oilIsLow ? HIGH : LOW);
    }
  }
}

// Function to set up glow plug related pins
void setupGlowPlug() {
  // Set glow plug pin as an output
  pinMode(GLOW_PLUG_TRANSISTOR_PIN, OUTPUT);
  digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, LOW); // Ensure glow plug is off initially

  // Set glow plug button pin as an input with internal pull-up resistor
  pinMode(GLOW_PLUG_BUTTON_PIN, INPUT_PULLUP);
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Engine Control Unit - Starting up...");

  setupGlowPlug();
  setupOilPressure();

  Serial.println("Setup complete. Waiting for button press.");
}

void loop() {
  handleGlowPlug(); 
  handleOilPressure();
}
