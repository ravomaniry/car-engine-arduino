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
                    │                    ARDUINO NANO ATmega328                │
                    │                                                         │
                    │  D2 ──────[MOSFET]───────────────────────────────────── │
                    │  D3 ──────[BUTTON]───────────────────────────────────── │
                    │  D4 ──────[1kΩ]───[OIL SWITCH]──────────────────────── │
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
                    │  D2 → N-MOSFET Gate → MOSFET Drain → Glow Plug (+)     │
                    │  MOSFET Source → GND                                   │
                    │  Glow Plug (-) → GND                                   │
                    │                                                         │
                    │  OIL PRESSURE SYSTEM:                                  │
                    │  D4 → 1kΩ Resistor → Oil Switch Pin                   │
                    │  Oil Switch → GND (normally closed)                    │
                    │                                                         │
                    │  TEMPERATURE SENSOR (NTC Thermistor):                 │
                    │  5V → 10kΩ Pull-up → A0 → Temp Sensor Signal          │
                    │  GND → Temp Sensor Ground                              │
                    │  100nF Capacitor: A0 to GND                            │
                    │  1N4007 Diode: A0 to 5V (protection)                  │
                    │                                                         │
                    │  FUEL LEVEL SENSOR (Resistive - 2 pins):              │
                    │  A1 → Fuel Sensor Pin 1                               │
                    │  GND → Fuel Sensor Pin 2                              │
                    │  10kΩ Pull-up: A1 to 5V                               │
                    │  100nF Capacitor: A1 to GND                            │
                    │  1N4007 Diode: A1 to 5V (protection)                  │
                    │                                                         │
                    │  I2C LCD DISPLAY (16x2):                               │
                    │  5V → LCD VCC                                          │
                    │  GND → LCD GND                                         │
                    │  A4 → LCD SDA                                          │
                    │  A5 → LCD SCL                                          │
                    │                                                         │
                    │  SERIAL COMMUNICATION:                                 │
                    │  TX (D1) → External Display Unit                       │
                    │  RX (D0) → External Display Unit                       │
                    └─────────────────────────────────────────────────────────┘
```

## Pin Assignments

| Pin | Function            | Color  | Description            |
| --- | ------------------- | ------ | ---------------------- |
| D2  | Glow Plug Control   | GREEN  | MOSFET gate control    |
| D3  | Glow Plug Button    | RED    | Manual activation      |
| D4  | Oil Pressure Switch | BLUE   | Pressure monitoring    |
| A0  | Temperature Sensor  | ORANGE | NTC thermistor input   |
| A1  | Fuel Level Sensor   | YELLOW | Resistive sensor input |
| A4  | I2C SDA             | -      | LCD data line          |
| A5  | I2C SCL             | -      | LCD clock line         |
| TX  | Serial TX           | -      | Data transmission      |
| RX  | Serial RX           | -      | Data reception         |

## Sensor Specifications

**Temperature Sensor:**

- Type: NTC Thermistor
- Resistance: 2.61kΩ (off) → 2.41kΩ (running)
- Voltage: ~2.05V (off) → ~1.95V (running)
- ADC: ~419 (off) → ~399 (running)

**Fuel Level Sensor:**

- Type: Resistive float sensor
- Current Reading: 122Ω (some fuel)
- Voltage: ~0.06V (with 10kΩ pull-up only)
- ADC: ~12
- Note: Need empty/full tank measurements for complete calibration

## Installation Notes

1. **Disconnect OEM 12V power** from both sensors
2. **Power sensors with Arduino 5V** instead
3. **Temperature sensor**: Located on thermostat housing (2 pins)
4. **Fuel sensor**: Located in fuel tank sender unit (2 pins)
5. **Wiring colors**: Yellow signal, green/yellow ground (verify with multimeter)
6. **Safety**: Ensure proper fuel system isolation before installation

## Software Upload

```bash
# Using PlatformIO
pio run --target upload

# Or using Arduino IDE
# Select Arduino Nano ATmega328 (New Bootloader)
# Upload the code
```

## Operation

1. Power on system
2. LCD shows startup message, then current status
3. Press glow plug button (D3) to activate preheating
4. Monitor LCD display and serial output
5. Oil pressure monitored continuously

## Troubleshooting

**Temperature Sensor:**

- Check 5V/GND connections
- Verify 10kΩ pull-up resistor
- Test voltage at A0 (~1.95-2.05V)
- Check resistance (~2.41-2.61kΩ)

**Fuel Sensor:**

- Verify ~122Ω resistance
- Test voltage at A1 (~0.06V with 10kΩ pull-up only)
- Check 10kΩ pull-up resistor (A1 to 5V)
- Measure empty/full tank for calibration

**General:**

- Check all ground connections
- Verify power supply (5V)
- Test with multimeter
- Check I2C LCD address (0x27)

---

**Note**: Designed for educational purposes. Ensure compliance with automotive safety standards for production use.
