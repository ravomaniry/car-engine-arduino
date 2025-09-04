#include <Arduino.h>
#include "glow_plug.h"
#include "oil_pressure.h"
#include "sensors.h"
#include "communication.h"

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Engine Control Unit - Starting up...");

  // Initialize all modules
  setupGlowPlug();
  setupOilPressure();
  
  // Initialize sensors and communication
  initializeSensors();
  initializeCommunication();
  
  Serial.println("Setup complete. System ready.");
}

void loop() {
  // Handle all subsystems
  handleGlowPlug(); 
  handleOilPressure();
  
  // Update sensors periodically
  updateSensors();
}
