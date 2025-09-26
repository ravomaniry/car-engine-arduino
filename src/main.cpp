#include <Arduino.h>
#include "glow_plug.h"
#include "oil_pressure.h"
#include "temperature_sensor.h"
#include "fuel_sensor.h"
#include "communication.h"
#include "lcd_display.h"

// Timing for sensor readings
const unsigned long SENSOR_UPDATE_INTERVAL_MS = 1000; // 1 second
static unsigned long lastSensorUpdateTime = 0;

// Wrapper functions for compatibility with existing code
int readCoolantSensor() {
  return readTemperatureSensor();
}

int readFuelSensor() {
  return readFuelLevel();
}

void initializeSensors() {
  // Initialize individual sensor modules
  initializeTemperatureSensor();
  initializeFuelSensor();
  
  // Initialize sensor values
  updateCoolantState(readCoolantSensor());
  updateFuelState(readFuelSensor());
}

void updateSensors() {
  // Read other sensors and update state every second
  if (millis() - lastSensorUpdateTime >= SENSOR_UPDATE_INTERVAL_MS) {
    updateCoolantState(readCoolantSensor());
    updateFuelState(readFuelSensor());
    
    lastSensorUpdateTime = millis(); // Update the timer
  }
}

void setup() {
  // Initialize serial communication for ESP32 communication
  Serial.begin(115200);
  Serial.println("Engine Control Unit - Starting up...");

  // Initialize all modules
  setupGlowPlug();
  setupOilPressure();
  
  // Initialize sensors and communication
  initializeSensors();
  initializeCommunication();
  
  // Initialize LCD display
  setupLCD();
  
  Serial.println("Setup complete. System ready.");
}

void loop() {
  // Handle all subsystems
  handleGlowPlug(); 
  handleOilPressure();
  
  // Update sensors periodically
  updateSensors();
  
  // Update LCD display
  updateLCD();
}
