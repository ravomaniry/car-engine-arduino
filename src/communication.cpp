#include "communication.h"

// State tracking variables
static SensorState currentState = {1, 0, 0, false}; // Initialize with default values
static SensorState lastSentState = {-1, -1, -1, false}; // Initialize with different values to force initial send

// Functions to send individual sensor data when changed or forced
static void sendOilData(bool force = false) {
  if (force || currentState.oil != lastSentState.oil) {
    Serial.print("OIL:");
    Serial.println(currentState.oil);
    lastSentState.oil = currentState.oil;
  }
}

static void sendCoolantData(bool force = false) {
  if (force || currentState.coolant != lastSentState.coolant) {
    Serial.print("COOLANT:");
    Serial.println(currentState.coolant);
    lastSentState.coolant = currentState.coolant;
  }
}

static void sendFuelData(bool force = false) {
  if (force || currentState.fuel != lastSentState.fuel) {
    Serial.print("FUEL:");
    Serial.println(currentState.fuel);
    lastSentState.fuel = currentState.fuel;
  }
}

static void sendGlowData(bool force = false) {
  if (force || currentState.glowActive != lastSentState.glowActive) {
    Serial.print("GLOW:");
    Serial.println(currentState.glowActive ? 1 : 0);
    lastSentState.glowActive = currentState.glowActive;
  }
}

void initializeCommunication() {
  // Send initial sensor data to display microcontroller
  Serial.println("Sending initial sensor data...");
  sendAllSensorData(true);
}

void sendAllSensorData(bool force) {
  sendOilData(force);
  sendCoolantData(force);
  sendFuelData(force);
  sendGlowData(force);
}

// Individual sensor update functions
void updateOilState(int oilValue) {
  currentState.oil = oilValue;
  sendOilData(); // Send immediately when changed
}

void updateCoolantState(int coolantValue) {
  currentState.coolant = coolantValue;
  sendCoolantData(); // Send immediately when changed
}

void updateFuelState(int fuelValue) {
  currentState.fuel = fuelValue;
  sendFuelData(); // Send immediately when changed
}

void updateGlowState(bool glowActive) {
  currentState.glowActive = glowActive;
  sendGlowData(); // Send immediately when changed
}
