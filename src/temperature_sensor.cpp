#include "temperature_sensor.h"

// Temperature sensor configuration
const int TEMP_SENSOR_PIN = A0;                    // Analog pin for temperature sensor
const float TEMP_SENSOR_MIN_VOLTAGE = 0.0;         // Minimum voltage
const float TEMP_SENSOR_MAX_VOLTAGE = 5.0;         // Maximum voltage
const int TEMP_SENSOR_MIN_TEMP = -40;              // Minimum temperature (°C)
const int TEMP_SENSOR_MAX_TEMP = 150;              // Maximum temperature (°C)

// Calibration values for Citroën Berlingo 1997 XUD engine coolant temperature sensor
// Uses thermistor (NTC) - resistance decreases as temperature increases
// Measured: 1682Ω at 20°C (actual direct measurement)
// Pull-up resistor calculated from voltage divider: 7.98kΩ
const float TEMP_SENSOR_PULLUP_RESISTOR = 7980.0;     // Pull-up resistor (calculated from voltage divider)
const float TEMP_SENSOR_BETA_COEFFICIENT = 3950.0;    // Beta coefficient for thermistor (typical for automotive NTC)
const float TEMP_SENSOR_NOMINAL_TEMP = 20.0;          // Nominal temperature (°C) - actual measurement
const float TEMP_SENSOR_NOMINAL_RESISTANCE = 1682.0;  // Resistance at 20°C (actual measured value)

// Filtering for stable readings
const int TEMP_FILTER_SIZE = 5;                    // Number of samples for averaging
int tempReadings[TEMP_FILTER_SIZE];
int tempReadingIndex = 0;
bool tempFilterInitialized = false;

/**
 * Initialize the temperature sensor
 */
void initializeTemperatureSensor() {
  pinMode(TEMP_SENSOR_PIN, INPUT);
  
  // Initialize filter array
  for (int i = 0; i < TEMP_FILTER_SIZE; i++) {
    tempReadings[i] = 0;
  }
  tempFilterInitialized = false;
  
  // Take initial reading to stabilize
  delay(100);
  readTemperatureSensor();
}

/**
 * Read raw ADC value from temperature sensor
 * @return Raw ADC value (0-1023)
 */
int readTemperatureSensorRaw() {
  return analogRead(TEMP_SENSOR_PIN);
}

/**
 * Read temperature in Celsius with filtering
 * @return Temperature in Celsius
 */
int readTemperatureSensor() {
  // Read raw ADC value
  int rawValue = readTemperatureSensorRaw();
  
  // Add to filter array
  tempReadings[tempReadingIndex] = rawValue;
  tempReadingIndex = (tempReadingIndex + 1) % TEMP_FILTER_SIZE;
  
  // Calculate average if filter is initialized
  if (!tempFilterInitialized) {
    tempFilterInitialized = true;
    return mapTemperature(rawValue);
  }
  
  // Calculate average of filtered readings
  long sum = 0;
  for (int i = 0; i < TEMP_FILTER_SIZE; i++) {
    sum += tempReadings[i];
  }
  int averageValue = sum / TEMP_FILTER_SIZE;
  
  return mapTemperature(averageValue);
}

/**
 * Map ADC value to temperature in Celsius (for thermistor)
 * @param adcValue Raw ADC value (0-1023)
 * @return Temperature in Celsius
 */
int mapTemperature(int adcValue) {
  // Convert ADC value to voltage
  float voltage = (adcValue * 5.0) / 1023.0;
  
  // Calculate thermistor resistance using voltage divider formula
  // Wiring: 5V → Pull-up → Analog Pin → Thermistor → Ground
  // V_out = V_supply * R_thermistor / (R_pullup + R_thermistor)
  // Solving for R_thermistor: R_thermistor = R_pullup * V_out / (V_supply - V_out)
  float thermistorResistance = TEMP_SENSOR_PULLUP_RESISTOR * voltage / (5.0 - voltage);
  
  // Use Steinhart-Hart equation for thermistor temperature calculation
  // 1/T = 1/T0 + (1/B) * ln(R/R0)
  // Where T0 = nominal temperature, R0 = nominal resistance, B = beta coefficient
  float temperature = 1.0 / (1.0 / (TEMP_SENSOR_NOMINAL_TEMP + 273.15) + 
                            (1.0 / TEMP_SENSOR_BETA_COEFFICIENT) * 
                            log(thermistorResistance / TEMP_SENSOR_NOMINAL_RESISTANCE));
  
  // Convert from Kelvin to Celsius
  temperature = temperature - 273.15;
  
  // Clamp temperature to valid range
  if (temperature < TEMP_SENSOR_MIN_TEMP) {
    temperature = TEMP_SENSOR_MIN_TEMP;
  }
  if (temperature > TEMP_SENSOR_MAX_TEMP) {
    temperature = TEMP_SENSOR_MAX_TEMP;
  }
  
  return (int)temperature;
}

/**
 * Read temperature sensor voltage
 * @return Voltage in volts (0-5V)
 */
float readTemperatureSensorVoltage() {
  int adcValue = readTemperatureSensorRaw();
  return (adcValue * 5.0) / 1023.0;
}

/**
 * Read temperature in Fahrenheit
 * @return Temperature in Fahrenheit
 */
int readTemperatureSensorFahrenheit() {
  int celsius = readTemperatureSensor();
  return (celsius * 9.0 / 5.0) + 32;
}

/**
 * Get temperature sensor status
 * @return true if sensor is working properly, false if there's an issue
 */
bool getTemperatureSensorStatus() {
  float voltage = readTemperatureSensorVoltage();
  
  // Check if voltage is within expected range
  if (voltage < 0.1 || voltage > 4.9) {
    return false; // Sensor disconnected or faulty
  }
  
  return true;
}

/**
 * Get temperature description
 * @param temperature Temperature in Celsius
 * @return String description of temperature level
 */
String getTemperatureDescription(int temperature) {
  if (temperature < 0) {
    return "FREEZING";
  } else if (temperature < 20) {
    return "COLD";
  } else if (temperature < 40) {
    return "NORMAL";
  } else if (temperature < 60) {
    return "WARM";
  } else if (temperature < 80) {
    return "HOT";
  } else if (temperature < 100) {
    return "VERY HOT";
  } else {
    return "CRITICAL";
  }
}

/**
 * Check if temperature is in normal operating range
 * @param temperature Temperature in Celsius
 * @return true if temperature is normal, false if warning/critical
 */
bool isTemperatureNormal(int temperature) {
  return (temperature >= 20 && temperature <= 80);
}

/**
 * Check if temperature requires immediate attention
 * @param temperature Temperature in Celsius
 * @return true if temperature is critical, false if safe
 */
bool isTemperatureCritical(int temperature) {
  return (temperature > 100);
}
