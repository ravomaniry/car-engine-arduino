#include "sensors.h"
#include "communication.h"

// Timing for sensor readings
const unsigned long SENSOR_UPDATE_INTERVAL_MS = 1000; // 1 second
static unsigned long lastSensorUpdateTime = 0;

// Placeholder functions for reading sensors (to be implemented with actual sensors)
int readCoolantSensor() {
  // TODO: Implement actual coolant temperature sensor reading
  return 75; // Default coolant temperature
}

int readFuelSensor() {
  // TODO: Implement actual fuel level sensor reading
  return 50; // Default fuel level
}

void initializeSensors() {
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
