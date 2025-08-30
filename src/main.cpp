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

// State tracking for sensor data transmission
struct SensorState {
  int speed;
  int oil;
  int coolant;
  int fuel;
  bool glowActive;
};

SensorState currentState = {0, 1, 0, 0, false}; // Initialize with default values
SensorState lastSentState = {-1, -1, -1, -1, false}; // Initialize with different values to force initial send

// Timing for sensor readings
const unsigned long SENSOR_UPDATE_INTERVAL_MS = 1000; // 1 second
unsigned long lastSensorUpdateTime = 0;

// Functions to send individual sensor data when changed or forced
void sendSpeedData(bool force = false) {
  if (force || currentState.speed != lastSentState.speed) {
    Serial.print("SPEED:");
    Serial.println(currentState.speed);
    lastSentState.speed = currentState.speed;
  }
}

void sendOilData(bool force = false) {
  if (force || currentState.oil != lastSentState.oil) {
    Serial.print("OIL:");
    Serial.println(currentState.oil);
    lastSentState.oil = currentState.oil;
  }
}

void sendCoolantData(bool force = false) {
  if (force || currentState.coolant != lastSentState.coolant) {
    Serial.print("COOLANT:");
    Serial.println(currentState.coolant);
    lastSentState.coolant = currentState.coolant;
  }
}

void sendFuelData(bool force = false) {
  if (force || currentState.fuel != lastSentState.fuel) {
    Serial.print("FUEL:");
    Serial.println(currentState.fuel);
    lastSentState.fuel = currentState.fuel;
  }
}

void sendGlowData(bool force = false) {
  if (force || currentState.glowActive != lastSentState.glowActive) {
    Serial.print("GLOW:");
    Serial.println(currentState.glowActive ? 1 : 0);
    lastSentState.glowActive = currentState.glowActive;
  }
}

// Placeholder functions for reading sensors (to be implemented with actual sensors)
int readSpeedSensor() {
  // TODO: Implement actual speed sensor reading
  return 0; // Default speed
}

int readCoolantSensor() {
  // TODO: Implement actual coolant temperature sensor reading
  return 75; // Default coolant temperature
}

int readFuelSensor() {
  // TODO: Implement actual fuel level sensor reading
  return 50; // Default fuel level
}

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
      currentState.glowActive = true; // Update state
      sendGlowData(); // Send glow status change
      Serial.println("Glow plug activated!");
    }
  }

  // If the glow plug is active, check if the glow time has elapsed
  if (glowPlugIsActive) {
    if (millis() - glowStartTime >= GLOW_TIME_SECONDS * 1000) { // Convert seconds to milliseconds
      glowPlugIsActive = false;
      digitalWrite(GLOW_PLUG_TRANSISTOR_PIN, LOW); // Turn off the glow plug
      currentState.glowActive = false; // Update state
      sendGlowData(); // Send glow status change
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

      // Update sensor state (0 = low oil pressure, 1 = normal oil pressure)
      currentState.oil = oilIsLow ? 0 : 1;

      // Send oil data immediately when it changes
      sendOilData();
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

  // Initialize sensor values
  currentState.speed = readSpeedSensor();
  currentState.coolant = readCoolantSensor();
  currentState.fuel = readFuelSensor();
  
  // Send initial sensor data to display microcontroller
  Serial.println("Sending initial sensor data...");
  sendSpeedData(true);    // Force send at startup
  sendOilData(true);      // Force send at startup
  sendCoolantData(true);  // Force send at startup
  sendFuelData(true);     // Force send at startup
  sendGlowData(true);     // Force send at startup
  
  Serial.println("Setup complete. Waiting for button press.");
}

void loop() {
  handleGlowPlug(); 
  handleOilPressure();
  
  // Read other sensors and update state every second
  if (millis() - lastSensorUpdateTime >= SENSOR_UPDATE_INTERVAL_MS) {
    currentState.speed = readSpeedSensor();
    sendSpeedData();
    
    currentState.coolant = readCoolantSensor();
    sendCoolantData();
    
    currentState.fuel = readFuelSensor();
    sendFuelData();
    
    lastSensorUpdateTime = millis(); // Update the timer
  }
}
