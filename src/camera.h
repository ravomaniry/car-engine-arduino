#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

// Camera configuration
extern const int CAMERA_MOSFET_PIN;
extern const int CAMERA_BUTTON_PIN;
extern const unsigned long CAMERA_BUTTON_DEBOUNCE_MS;
extern const unsigned long CAMERA_AUTO_OFF_TIMEOUT_MS;
extern const unsigned long CAMERA_MANUAL_TIMEOUT_MS;

// Camera functions
void setupCamera();
void handleCamera();
void activateCameraByReverse();
void deactivateCameraByReverse();
bool isCameraActive();

#endif
