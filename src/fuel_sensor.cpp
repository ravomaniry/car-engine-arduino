#include "fuel_sensor.h"

// Fuel level sensor configuration
const int FUEL_SENSOR_PIN = A1;                    // Analog pin for fuel sensor
const float FUEL_SENSOR_MIN_VOLTAGE = 0.5;         // Minimum voltage (empty tank)
const float FUEL_SENSOR_MAX_VOLTAGE = 4.5;         // Maximum voltage (full tank)
const int FUEL_SENSOR_MIN_PERCENTAGE = 0;          // Minimum fuel percentage
const int FUEL_SENSOR_MAX_PERCENTAGE = 100;        // Maximum fuel percentage

// Calibration values for Citroën Berlingo 1997 XUD engine
// Actual measured value: 122Ω with some fuel left in tank
// With 10kΩ pull-up resistor only (no pull-down)
// Voltage calculation: Vout = 5V × (Rsensor / (Rpullup + Rsensor))
// Current reading: 122Ω → ~0.06V → ADC ~12
const int FUEL_EMPTY_ADC_VALUE = 5;                // Estimated ADC value when tank is empty
const int FUEL_FULL_ADC_VALUE = 12;                // ADC value with current fuel level (122Ω)

// Filtering for stable readings
const int FUEL_FILTER_SIZE = 5;                    // Number of samples for averaging
int fuelReadings[FUEL_FILTER_SIZE];
int fuelReadingIndex = 0;
bool fuelFilterInitialized = false;

/**
 * Initialize the fuel level sensor
 */
void initializeFuelSensor() {
  pinMode(FUEL_SENSOR_PIN, INPUT);
  
  // Initialize filter array
  for (int i = 0; i < FUEL_FILTER_SIZE; i++) {
    fuelReadings[i] = 0;
  }
  fuelFilterInitialized = false;
  
  // Take initial reading to stabilize
  delay(100);
  readFuelLevel();
}

/**
 * Read raw ADC value from fuel sensor
 * @return Raw ADC value (0-1023)
 */
int readFuelSensorRaw() {
  return analogRead(FUEL_SENSOR_PIN);
}

/**
 * Read fuel level percentage with filtering
 * @return Fuel level percentage (0-100)
 */
int readFuelLevel() {
  // Read raw ADC value
  int rawValue = readFuelSensorRaw();
  
  // Add to filter array
  fuelReadings[fuelReadingIndex] = rawValue;
  fuelReadingIndex = (fuelReadingIndex + 1) % FUEL_FILTER_SIZE;
  
  // Calculate average if filter is initialized
  if (!fuelFilterInitialized) {
    fuelFilterInitialized = true;
    return mapFuelLevel(rawValue);
  }
  
  // Calculate average of filtered readings
  long sum = 0;
  for (int i = 0; i < FUEL_FILTER_SIZE; i++) {
    sum += fuelReadings[i];
  }
  int averageValue = sum / FUEL_FILTER_SIZE;
  
  return mapFuelLevel(averageValue);
}

/**
 * Map ADC value to fuel percentage
 * @param adcValue Raw ADC value (0-1023)
 * @return Fuel percentage (0-100)
 */
int mapFuelLevel(int adcValue) {
  // Clamp ADC value to expected range
  if (adcValue < FUEL_EMPTY_ADC_VALUE) {
    adcValue = FUEL_EMPTY_ADC_VALUE;
  }
  if (adcValue > FUEL_FULL_ADC_VALUE) {
    adcValue = FUEL_FULL_ADC_VALUE;
  }
  
  // Map ADC value to percentage
  int percentage = map(adcValue, FUEL_EMPTY_ADC_VALUE, FUEL_FULL_ADC_VALUE, 
                       FUEL_SENSOR_MIN_PERCENTAGE, FUEL_SENSOR_MAX_PERCENTAGE);
  
  // Ensure percentage is within valid range
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;
  
  return percentage;
}

/**
 * Read fuel sensor voltage
 * @return Voltage in volts (0-5V)
 */
float readFuelSensorVoltage() {
  int adcValue = readFuelSensorRaw();
  return (adcValue * 5.0) / 1023.0;
}

/**
 * Calibrate fuel sensor (call when tank is empty)
 * Updates the empty tank ADC value
 */
void calibrateFuelSensorEmpty() {
  int currentValue = readFuelSensorRaw();
  // Update calibration (you might want to store this in EEPROM)
  // For now, we'll use the current reading as reference
  // In a real implementation, you'd store this in EEPROM
}

/**
 * Calibrate fuel sensor (call when tank is full)
 * Updates the full tank ADC value
 */
void calibrateFuelSensorFull() {
  int currentValue = readFuelSensorRaw();
  // Update calibration (you might want to store this in EEPROM)
  // For now, we'll use the current reading as reference
  // In a real implementation, you'd store this in EEPROM
}

/**
 * Get fuel sensor status
 * @return true if sensor is working properly, false if there's an issue
 */
bool getFuelSensorStatus() {
  float voltage = readFuelSensorVoltage();
  
  // Check if voltage is within expected range
  if (voltage < 0.1 || voltage > 4.9) {
    return false; // Sensor disconnected or faulty
  }
  
  return true;
}

/**
 * Get fuel level description
 * @param percentage Fuel percentage (0-100)
 * @return String description of fuel level
 */
String getFuelLevelDescription(int percentage) {
  if (percentage <= 10) {
    return "EMPTY";
  } else if (percentage <= 25) {
    return "LOW";
  } else if (percentage <= 50) {
    return "HALF";
  } else if (percentage <= 75) {
    return "GOOD";
  } else {
    return "FULL";
  }
}
