#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

// State tracking for sensor data transmission
struct SensorState {
  int oil;
  int coolant;
  int fuel;
  bool glowActive;
  bool cameraActive;
  bool reverseGear;
};

// Communication functions
void initializeCommunication();
void sendAllSensorData(bool force = false);

// Individual sensor update functions
void updateOilState(int oilValue);
void updateCoolantState(int coolantValue);
void updateFuelState(int fuelValue);
void updateGlowState(bool glowActive);
void updateCameraState(bool cameraActive);
void updateReverseGearState(bool reverseGear);

#endif
