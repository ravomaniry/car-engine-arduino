#include "lcd_display.h"
#include "glow_plug.h"
#include "oil_pressure.h"
#include "sensors.h"

// I2C LCD configuration (only 2 pins: SDA=A4, SCL=A5)
const int LCD_I2C_ADDRESS = 0x27; // Common I2C address for LCD modules

// Initialize I2C LCD object (16 columns, 2 rows)
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);

// Display update timing and state tracking
const unsigned long LCD_UPDATE_INTERVAL_MS = 1000; // Check for updates every 1 second
static unsigned long lastLCDUpdate = 0;

// Track previous values to detect changes
static bool lastOilLow = false;
static bool lastGlowActive = false;
static int lastTemperature = 0;

void setupLCD() {
  // Initialize I2C communication
  Wire.begin();
  
  // Initialize the LCD with 16 columns and 2 rows
  lcd.init();
  lcd.backlight(); // Turn on backlight
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Engine Control");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  delay(2000); // Show startup message for 2 seconds
  
  // Clear display and show initial status
  lcd.clear();
  updateLCD();
}

void updateLCD() {
  // Check for updates every LCD_UPDATE_INTERVAL_MS milliseconds
  if (millis() - lastLCDUpdate < LCD_UPDATE_INTERVAL_MS) {
    return;
  }
  
  lastLCDUpdate = millis();
  
  // Get current values
  bool currentOilLow = isOilLow();
  bool currentGlowActive = isGlowPlugActive();
  int currentTemperature = readCoolantSensor();
  
  // Check if any values have changed
  bool needsUpdate = (currentOilLow != lastOilLow) || 
                     (currentGlowActive != lastGlowActive) || 
                     (currentTemperature != lastTemperature);
  
  // Only update display if something changed
  if (needsUpdate) {
    // Clear the display
    lcd.clear();
    
    // Display oil status on the left (first 5 characters)
    displayOilStatus(currentOilLow);
    
    // Display glow plug status in the center only when active
    if (currentGlowActive) {
      displayGlowPlugStatus(true);
    }
    
    // Display temperature on the right (characters 13-15, right-aligned)
    displayTemperature(currentTemperature);
    
    // Update stored values
    lastOilLow = currentOilLow;
    lastGlowActive = currentGlowActive;
    lastTemperature = currentTemperature;
  }
}

void displayOilStatus(bool isLow) {
  lcd.setCursor(0, 0);
  lcd.print("OIL");
  lcd.setCursor(0, 1);
  if (isLow) {
    lcd.print("LOW ");
  } else {
    lcd.print("OK  ");
  }
}

void displayGlowPlugStatus(bool isActive) {
  lcd.setCursor(6, 0);
  lcd.print("GLOW");
  lcd.setCursor(6, 1);
  lcd.print("ON  ");
}

void displayTemperature(int temperature) {
  lcd.setCursor(12, 0);
  lcd.print("TEMP");
  
  // Right-align temperature value to touch the right edge
  String tempStr = String(temperature) + "C";
  int startPos = 16 - tempStr.length(); // Calculate position to right-align
  lcd.setCursor(startPos, 1);
  lcd.print(tempStr);
}
