# Engine Control Unit (ECU) - Arduino Project

Engine Control Unit for 1997 Citroën Berlingo XUD engine using Arduino Nano ATmega328.

## Features

- **Glow Plug Control**: Manual activation with 5-second timeout
- **Oil Pressure Monitoring**: Real-time switch monitoring
- **Temperature Sensor**: NTC thermistor (2.61kΩ off, 2.41kΩ running)
- **Fuel Level Sensor**: Resistive sensor (122Ω measured)
- **LCD Display**: 16x2 I2C display for local monitoring
- **Serial Communication**: Data transmission to external systems

## Complete Wiring Diagram

```
┌─────────────────────────────────────────────────────────┐
│                    ARDUINO NANO ATmega328               │
│                                                         │
│  D2 ──────[MOSFET]───────────────────────────────────── │
│  D3 ──────[BUTTON]───────────────────────────────────── │
│  D4 ──────[1kΩ]───[OIL SWITCH]────────────────────────  │
│                                                         │
│  A0 ──────[10kΩ]───[TEMP SENSOR]─────────────────────── │
│  A1 ──────[10kΩ]───[FUEL SENSOR]─────────────────────── │
│  A4 ──────[I2C LCD SDA]──────────────────────────────── │
│  A5 ──────[I2C LCD SCL]──────────────────────────────── │
│                                                         │
│  5V ──────[SENSOR POWER]─────────────────────────────── │
│  GND ─────[COMMON GROUND]────────────────────────────── │
│  TX ──────[SERIAL OUT]───────────────────────────────── │
│  RX ──────[SERIAL IN]────────────────────────────────── │
└─────────────────────────────────────────────────────────┘
                       │
                       │
┌─────────────────────────────────────────────────────────┐
│                    COMPONENT DETAILS                    │
│                                                         │
│  GLOW PLUG SYSTEM:                                      │
│  D2 → MOSFET 1                                          │
│  Glow Plug (-) → GND                                    │
│                                                         │
│  OIL PRESSURE SYSTEM:                                   │
│  D4 → 1kΩ Resistor → SENSOR PIN 7 -> Oil Switch         │
│  Oil Switch → GND (normally closed)                     │
│                                                         │
│  TEMPERATURE SENSOR (NTC Thermistor):                   │
│  5V   → 10kΩ Pull-up    → A0 → PIN 6 -> Temp Sensor     │
│  GND  → Temp Sensor GND                                 │
│  100nF Capacitor: A0 to GND                             │
│  1N4007 Diode: A0 to 5V (protection)                    │
│                                                         │
│  FUEL LEVEL SENSOR (Resistive - 2 pins):                │
│  A1   → PIN 5             -> Fuel Sensor Pin 1          │
│  GND  → Fuel Sensor Pin 2                               │
│  10kΩ Pull-up: A1 to 5V                                 │
│  100nF Capacitor: A1 to GND                             │
│  1N4007 Diode: A1 to 5V (protection)                    │
│                                                         │
│  I2C LCD DISPLAY (16x2):                                │
│  5V → LCD VCC                                           │
│  GND → LCD GND                                          │
│  A4 → LCD SDA                                           │
│  A5 → LCD SCL                                           │
│                                                         │
│  SERIAL COMMUNICATION:                                  │
│  TX (D1) → External Display Unit                        │
│  RX (D0) → External Display Unit                        │
└─────────────────────────────────────────────────────────┘
```
