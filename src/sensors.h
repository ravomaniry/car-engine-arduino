#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// Sensor timing configuration
extern const unsigned long SENSOR_UPDATE_INTERVAL_MS;

// Sensor functions
void initializeSensors();
void updateSensors();
int readCoolantSensor();
int readFuelSensor();

#endif
