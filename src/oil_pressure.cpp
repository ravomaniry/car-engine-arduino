#include "oil_pressure.h"
#include "communication.h"

// Oil pressure configuration
const byte OIL_SWITCH_PIN = 4; // D4: BLUE
const unsigned long OIL_DEBOUNCE_MS = 100;

// Oil pressure state variables
static bool oilIsLow = false;
static uint8_t oilLastRawReading = HIGH;
static bool oilStableState = HIGH;          // debounced state
static unsigned long oilLastChangeMillis = 0;

void setupOilPressure() {
  pinMode(OIL_SWITCH_PIN, INPUT_PULLUP);  // use internal pull-up

  // read initial state
  oilLastRawReading = digitalRead(OIL_SWITCH_PIN);
  oilStableState = oilLastRawReading;
  // With 1k/10k resistor setup: LOW = low pressure (switch closed), HIGH = normal pressure (switch open)
  oilIsLow = (oilStableState == LOW);
  updateOilState(oilIsLow ? 1 : 0); // Initialize oil state (1 = low pressure warning, 0 = normal)

  oilLastChangeMillis = millis();
}

void handleOilPressure() {
  uint8_t raw = digitalRead(OIL_SWITCH_PIN);

  // if input changed, reset timer
  if (raw != oilLastRawReading) {
    oilLastChangeMillis = millis();
    oilLastRawReading = raw;
  } else {
    // if stable long enough and different from stable state → update
    if (raw != oilStableState && (millis() - oilLastChangeMillis >= OIL_DEBOUNCE_MS)) {
      oilStableState = raw;
      // With 1k/10k resistor setup: LOW = low pressure (switch closed), HIGH = normal pressure (switch open)
      oilIsLow = (oilStableState == LOW);

      // Update sensor state (1 = low oil pressure warning, 0 = normal oil pressure)
      updateOilState(oilIsLow ? 1 : 0);
    }
  }
}

bool isOilLow() {
  return oilIsLow;
}
