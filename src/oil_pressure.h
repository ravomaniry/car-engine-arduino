#ifndef OIL_PRESSURE_H
#define OIL_PRESSURE_H

#include <Arduino.h>

// Oil pressure configuration
extern const byte OIL_SWITCH_PIN;
extern const unsigned long OIL_DEBOUNCE_MS;

// Oil pressure functions
void setupOilPressure();
void handleOilPressure();
bool isOilLow();

#endif
