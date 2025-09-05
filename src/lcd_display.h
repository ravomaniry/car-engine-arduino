#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD configuration (only 2 pins: SDA and SCL)
extern const int LCD_I2C_ADDRESS;

// LCD display functions
void setupLCD();
void updateLCD();
void displayOilStatus(bool isLow);
void displayGlowPlugStatus(bool isActive);
void displayTemperature(int temperature);

#endif
