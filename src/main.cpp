#include <Arduino.h>
#include "glow_plug.h"
#include "oil_pressure.h"
#include "sensors.h"
#include "communication.h"
#include "lcd_display.h"

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
