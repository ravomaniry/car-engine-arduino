#include <Arduino.h>
#include "glow_plug.h"
#include "oil_pressure.h"
#include "reverse_gear.h"
#include "camera.h"
#include "sensors.h"
#include "communication.h"

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Engine Control Unit - Starting up...");

  // Initialize all modules
  setupGlowPlug();
  setupOilPressure();
  setupReverseGear();
  setupCamera();
  
  // Initialize sensors and communication
  initializeSensors();
  initializeCommunication();
  
  Serial.println("Setup complete. System ready.");
}

void loop() {
  // Handle all subsystems
  handleGlowPlug(); 
  handleOilPressure();
  handleReverseGear();
  handleCamera();
  
  // Update sensors periodically
  updateSensors();
}
