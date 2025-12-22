# SkyGlass FireStick IR Interceptor

This project allows you to control your **Sky Glass TV** using an **Amazon Fire TV Stick remote**. It utilizes an **ESP8266 or ESP32** microcontroller to intercept IR signals from the FireStick remote and translate them into the corresponding RC6 IR codes that the Sky Glass TV understands.

## Overview

The "SkyGlass FireStick IR Interceptor" listens for specific IR commands (Volume Up, Volume Down, Mute, Power) sent by a FireStick remote (NEC protocol) and instantly transmits the equivalent command for a Sky Glass TV (RC6 protocol). This bridges the compatibility gap between the two devices.

## Features

- **Protocol Translation**: Converts NEC IR signals (FireStick) to RC6 IR signals (Sky Glass).
- **Supported Commands**:
  - Power
  - Volume Up
  - Volume Down
  - Mute
- **Debug Output**: Prints received and sent codes to the Serial Monitor for easy debugging.

## Hardware Requirements

- **ESP8266/ESP32 Board** (e.g., NodeMCU, Wemos D1 Mini, ESP32 DevKit)
- **IR Receiver Module** (e.g., TSOP38238)
- **IR Transmitter LED** (or an IR Transmitter Module)
- **NPN Transistor** (if driving the IR LED directly, e.g., 2N2222) - *Optional but recommended for better range*
- **Resistor** (approx. 220Ω for LED protection)
- Jumper wires & Breadboard

## Software Requirements

- **Arduino IDE**
- **IRremoteESP8266 Library**: Install via the Arduino Library Manager.

## Wiring

| Component | ESP8266 Pin | ESP32 Pin (Default) | Description |
|-----------|-------------|---------------------|-------------|
| **IR Receiver** | D6 (GPIO 12) | GPIO 12 | Signal Output Pin |
| **IR LED** | D1 (GPIO 5) | GPIO 4 | Anode (+) via resistor |

*Note: Pin mappings can be adjusted in the code if needed. Ensure common ground between all components and the microcontroller.*

## Installation

1.  **Install the Library**:
    - Open Arduino IDE.
    - Go to `Sketch` -> `Include Library` -> `Manage Libraries...`
    - Search for `IRremoteESP8266` and install it.

2.  **Upload the Code**:
2.  **Upload the Code**:
    - Open `skyglass_firestick_ir_interceptor.ino` in Arduino IDE.
    - If using an **ESP32**, make sure to check/update the pin definitions in the code (e.g., change `D1` to `4` and `D6` to `12` or your preferred GPIOs).
    - Select your board model (ESP8266 or ESP32) in `Tools` -> `Board`.
    - Select the correct Port.
    - Click **Upload**.

3.  **Positioning**:
    - Place the **IR Receiver** so it has a clear line of sight to your FireStick remote.
    - Place the **IR LED** so it faces the IR receiver on your Sky Glass TV.

## Usage

1.  Power on the Microcontroller.
2.  Open the Serial Monitor (Baud Rate: `115200`) to view status logs.
3.  Point your FireStick remote at the setup and press a button (Vol+, Vol-, Mute, or Power).
4.  The device will display the received command and confirm if it's sending the corresponding Sky Glass command.

## Code Mapping

| Function | FireStick Code (NEC) | Sky Glass Code (RC6) |
|----------|----------------------|----------------------|
| Power | `0x12` | `0xC0081A0C` |
| Vol Up | `0x1A` | `0xC0081610` |
| Vol Down | `0x1E` | `0xC0081611` |
| Mute | `0x10` | `0xC008160D` |

## License

This project is licensed under the  GNU GENERAL PUBLIC LICENSE - see the [LICENSE](LICENSE) file for details.
