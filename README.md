
# Embedded Systems Communication Protocols

## Project Overview
This project compares the performance of different communication protocols for data transfer between two Arduino boards. The protocols covered include:

- **I2C (Inter-Integrated Circuit)**
- **SPI (Serial Peripheral Interface)**
- **UART (Universal Asynchronous Receiver-Transmitter)**
- **Wi-Fi (ESP8266 module)**
- **CAN (Controller Area Network)**
- **One-Wire Protocol**

Each protocol is implemented with a step-by-step guide, including hardware connections, Arduino code examples, and performance evaluation.

## Features
- Step-by-step implementation of each protocol
- Hardware connection schematics for Arduino boards
- Comparison of protocol performance in terms of speed, reliability, and complexity
- Code samples using Arduino libraries

## Hardware Requirements
To replicate this project, you will need:
- **2 Arduino Boards** (e.g., Arduino Uno, ESP32, etc.)
- **I2C-compatible devices** (e.g., sensors, EEPROMs)
- **SPI-supported devices** (e.g., SD cards, displays)
- **CAN Module (MCP2515)**
- **Wi-Fi module (ESP8266 or ESP32)**
- **One-Wire sensors** (e.g., DS18B20 temperature sensor)
- **Jumper wires and resistors (e.g., 4.7kΩ for One-Wire pull-up)**

## Protocol Implementations

### 1. I2C Protocol
- **Pins Used:** SDA (A4), SCL (A5)
- **Library:** `Wire.h`
- **Connections:**
  - SDA → SDA
  - SCL → SCL
  - GND → GND

### 2. SPI Protocol
- **Pins Used:** MOSI, MISO, SCK, SS
- **Library:** `SPI.h`
- **Connections:**
  - MOSI → MOSI
  - MISO → MISO
  - SCK → SCK
  - SS → SS
  - GND → GND

### 3. UART Protocol
- **Pins Used:** TX, RX
- **Library:** `Serial.begin()`
- **Connections:**
  - TX (Board 1) → RX (Board 2)
  - RX (Board 1) → TX (Board 2)
  - GND → GND

### 4. Wi-Fi Protocol (ESP8266/ESP32)
- **Library:** `WiFi.h`
- **Setup:** One board as a server, the other as a client

### 5. CAN Protocol
- **Module Used:** MCP2515 CAN Module
- **Library:** `mcp_can.h`
- **Connections:**
  - CAN_H → CAN_H
  - CAN_L → CAN_L
  - GND → GND

### 6. One-Wire Protocol
- **Sensor Used:** DS18B20 Temperature Sensor
- **Library:** `OneWire.h`
- **Connections:**
  - Data Line → Any digital pin
  - 4.7kΩ pull-up resistor between data line and VCC
  - GND → GND

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/Embedded-Communication-Protocols.git
   ```
2. Install necessary Arduino libraries:
   - `Wire.h` for I2C
   - `SPI.h` for SPI
   - `WiFi.h` for Wi-Fi
   - `mcp_can.h` for CAN
   - `OneWire.h` for One-Wire
3. Upload the respective protocol codes to the Arduino boards.

## Testing and Performance Evaluation
- Each protocol is tested by transmitting predefined data packets.
- Latency and transmission speed are compared.
- The results help determine the best protocol for specific use cases.

## Contribution
Feel free to contribute by submitting pull requests or reporting issues.

---

For more details, refer to the documentation files in the repository.

## Authors:
Mehrshad Dehghani : 401105912
Yousef Sadidi : 401170597
Amirali Sheikhi : 401106158
keyhan masoudi : 401106509



