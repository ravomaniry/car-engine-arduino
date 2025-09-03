#ifndef REVERSE_GEAR_H
#define REVERSE_GEAR_H

#include <Arduino.h>

// Reverse gear configuration
extern const byte REVERSE_GEAR_PIN;
extern const unsigned long REVERSE_GEAR_DEBOUNCE_MS;

// Reverse gear functions
void setupReverseGear();
void handleReverseGear();
bool isReverseGearEngaged();

#endif
