#ifndef FUEL_SENSOR_H
#define FUEL_SENSOR_H

#include <Arduino.h>

// Fuel sensor configuration constants
extern const int FUEL_SENSOR_PIN;
extern const float FUEL_SENSOR_MIN_VOLTAGE;
extern const float FUEL_SENSOR_MAX_VOLTAGE;
extern const int FUEL_SENSOR_MIN_PERCENTAGE;
extern const int FUEL_SENSOR_MAX_PERCENTAGE;

// Calibration constants
extern const int FUEL_EMPTY_ADC_VALUE;
extern const int FUEL_FULL_ADC_VALUE;

// Filtering constants
extern const int FUEL_FILTER_SIZE;

// Fuel sensor functions
void initializeFuelSensor();
int readFuelSensorRaw();
int readFuelLevel();
int mapFuelLevel(int adcValue);
float readFuelSensorVoltage();
void calibrateFuelSensorEmpty();
void calibrateFuelSensorFull();
bool getFuelSensorStatus();
String getFuelLevelDescription(int percentage);

#endif
