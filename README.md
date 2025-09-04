# Engine Control Unit (ECU) - Arduino Project

This project implements an Engine Control Unit using an Arduino Nano ATmega328 for monitoring and controlling various engine systems in a vehicle.

## Overview

The ECU monitors and controls:

- **Glow Plug System**: Manual activation with automatic timeout
- **Oil Pressure Monitoring**: Real-time oil pressure switch monitoring
- **Sensor Management**: Coolant temperature and fuel level sensors
- **Communication**: Serial data transmission to display systems

## Hardware Requirements

- **Arduino Nano ATmega328** (New Bootloader)
- **Transistor/MOSFET** for glow plug control
- **Oil pressure switch** (normally closed type)
- **Push button** for glow plug activation
- **Coolant temperature sensor** (analog)
- **Fuel level sensor** (analog)

## Pin Wiring Configuration

### Digital Pins

| Pin    | Function            | Color Code | Description              | Wiring                                                                     |
| ------ | ------------------- | ---------- | ------------------------ | -------------------------------------------------------------------------- |
| **D2** | Glow Plug Control   | GREEN      | Transistor/MOSFET gate   | Connect to gate of N-channel MOSFET/transistor for glow plug control       |
| **D3** | Glow Plug Button    | RED        | Manual activation button | Connect push button between pin and GND (internal pull-up enabled)         |
| **D4** | Oil Pressure Switch | BLUE       | Oil pressure monitoring  | Connect oil pressure switch between pin and GND (internal pull-up enabled) |

### Analog Pins (Future Implementation)

| Pin    | Function            | Description              | Wiring                                          |
| ------ | ------------------- | ------------------------ | ----------------------------------------------- |
| **A0** | Coolant Temperature | Temperature sensor input | Connect analog temperature sensor (0-5V output) |
| **A1** | Fuel Level          | Fuel level sensor input  | Connect analog fuel level sensor (0-5V output)  |

### Power and Communication

| Pin         | Function             | Description                       |
| ----------- | -------------------- | --------------------------------- |
| **VCC**     | Power Supply         | 5V or 3.3V (depending on sensors) |
| **GND**     | Ground               | Common ground for all components  |
| **TX (D1)** | Serial Communication | Data transmission to display unit |
| **RX (D0)** | Serial Communication | Data reception from display unit  |

## Wiring Diagram

```
Arduino Nano ATmega328
┌─────────────────────────────────┐
│  D2 (GREEN) ────[MOSFET]─────── Glow Plug (+)
│  D3 (RED) ──────[Button]─────── GND
│  D4 (BLUE) ─────[Oil Switch]─── GND
│  A0 ────────────[Temp Sensor]─── (Future)
│  A1 ────────────[Fuel Sensor]─── (Future)
│  TX ────────────[Display Unit]───
│  RX ────────────[Display Unit]───
│  VCC ─────────── 5V/3.3V
│  GND ─────────── Common Ground
└─────────────────────────────────┘
```

## Software Architecture

### Core Modules

#### 1. Glow Plug System (`glow_plug.cpp/h`)

- **Function**: Controls diesel engine glow plug preheating
- **Activation**: Manual button press (D3)
- **Control**: Transistor/MOSFET switching (D2)
- **Timeout**: Automatic 5-second cutoff
- **Debouncing**: 100ms switch debouncing

#### 2. Oil Pressure Monitoring (`oil_pressure.cpp/h`)

- **Function**: Monitors engine oil pressure
- **Input**: Digital switch on D4
- **Logic**: LOW = normal pressure, HIGH = low pressure
- **Debouncing**: 100ms switch debouncing
- **Alert**: Immediate state change notification

#### 3. Sensor Management (`sensors.cpp/h`)

- **Function**: Manages analog sensor readings
- **Update Rate**: 1-second intervals
- **Sensors**: Coolant temperature, fuel level
- **Status**: Currently using placeholder values

#### 4. Communication System (`communication.cpp/h`)

- **Function**: Serial data transmission
- **Protocol**: Simple text-based format
- **Format**: `SENSOR:VALUE` (e.g., `OIL:1`, `GLOW:0`)
- **Transmission**: On state change or forced update

## Serial Communication Protocol

The ECU sends data in the following format:

```
OIL:1          # Oil pressure (1=normal, 0=low)
COOLANT:75     # Coolant temperature (°C)
FUEL:50        # Fuel level (%)
GLOW:1         # Glow plug status (1=active, 0=inactive)
CAMERA:0       # Camera status (1=active, 0=inactive)
REVERSE:0      # Reverse gear status (1=active, 0=inactive)
```

## Configuration Constants

### Glow Plug System

```cpp
const unsigned long GLOW_TIME_SECONDS = 5;        // Glow duration
const int GLOW_PLUG_TRANSISTOR_PIN = 2;           // Control pin
const int GLOW_PLUG_BUTTON_PIN = 3;               // Button pin
const unsigned long GLOW_SWITCH_DEBOUNCE_MS = 100; // Debounce time
```

### Oil Pressure System

```cpp
const byte OIL_SWITCH_PIN = 4;                    // Switch pin
const unsigned long OIL_DEBOUNCE_MS = 100;        // Debounce time
```

### Sensor System

```cpp
const unsigned long SENSOR_UPDATE_INTERVAL_MS = 1000; // Update rate
```

## Installation and Usage

### 1. Hardware Setup

1. Connect components according to pin wiring table
2. Ensure proper power supply (5V or 3.3V)
3. Connect serial communication to display unit
4. Verify all ground connections

### 2. Software Upload

```bash
# Using PlatformIO
pio run --target upload

# Or using Arduino IDE
# Select Arduino Nano ATmega328 (New Bootloader)
# Upload the code
```

### 3. Operation

1. Power on the system
2. Press glow plug button (D3) to activate preheating
3. Monitor serial output for sensor data
4. Oil pressure is monitored continuously

## Safety Considerations

- **Glow Plug Control**: Ensure proper MOSFET/transistor rating for glow plug current
- **Oil Pressure**: Critical safety system - ensure proper switch wiring
- **Power Supply**: Use appropriate voltage and current capacity
- **Grounding**: Ensure all components share common ground
- **Fuses**: Consider adding fuses for protection

## Troubleshooting

### Common Issues

1. **Glow Plug Not Working**

   - Check MOSFET/transistor connections
   - Verify button wiring and pull-up resistor
   - Test with multimeter

2. **Oil Pressure False Readings**

   - Verify switch type (normally closed vs normally open)
   - Check switch wiring and ground connection
   - Test switch with multimeter

3. **Serial Communication Issues**

   - Check baud rate (9600)
   - Verify TX/RX connections
   - Test with serial monitor

4. **Power Issues**
   - Check voltage levels
   - Verify ground connections
   - Ensure adequate current capacity

## Future Enhancements

- [ ] Implement actual analog sensor readings
- [ ] Add reverse gear detection
- [ ] Add camera control system
- [ ] Implement CAN bus communication
- [ ] Add data logging capabilities
- [ ] Implement error handling and diagnostics

## Development Environment

- **Platform**: PlatformIO
- **Framework**: Arduino
- **Board**: Arduino Nano ATmega328 (New Bootloader)
- **IDE**: VS Code with PlatformIO extension

## License

This project is open source. Please ensure compliance with local regulations when implementing in vehicles.

---

**Note**: This ECU is designed for educational and experimental purposes. For production vehicle use, ensure compliance with automotive safety standards and regulations.
