#ifndef GLOW_PLUG_H
#define GLOW_PLUG_H

#include <Arduino.h>

// Glow plug configuration
extern const unsigned long GLOW_TIME_SECONDS;
extern const int GLOW_PLUG_TRANSISTOR_PIN;
extern const int GLOW_PLUG_BUTTON_PIN;
extern const unsigned long GLOW_SWITCH_DEBOUNCE_MS;

// Glow plug functions
void setupGlowPlug();
void handleGlowPlug();
bool isGlowPlugActive();
int getRemainingGlowTime();

#endif
