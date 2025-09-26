#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Arduino.h>

// Temperature sensor configuration constants
extern const int TEMP_SENSOR_PIN;
extern const float TEMP_SENSOR_MIN_VOLTAGE;
extern const float TEMP_SENSOR_MAX_VOLTAGE;
extern const int TEMP_SENSOR_MIN_TEMP;
extern const int TEMP_SENSOR_MAX_TEMP;

// Calibration constants for Berlingo XUD engine
extern const float TEMP_SENSOR_PULLUP_RESISTOR;
extern const float TEMP_SENSOR_BETA_COEFFICIENT;
extern const float TEMP_SENSOR_NOMINAL_TEMP;
extern const float TEMP_SENSOR_NOMINAL_RESISTANCE;

// Filtering constants
extern const int TEMP_FILTER_SIZE;

// Temperature sensor functions
void initializeTemperatureSensor();
int readTemperatureSensorRaw();
int readTemperatureSensor();
int mapTemperature(int adcValue);
float readTemperatureSensorVoltage();
int readTemperatureSensorFahrenheit();
bool getTemperatureSensorStatus();
String getTemperatureDescription(int temperature);
bool isTemperatureNormal(int temperature);
bool isTemperatureCritical(int temperature);

#endif
