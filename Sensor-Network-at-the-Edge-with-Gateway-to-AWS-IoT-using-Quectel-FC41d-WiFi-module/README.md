# Sensor Network at the Edge with Gateway to AWS‑IoT using Quectel FC41D

This repository contains an example sensor network that collects readings from one or more nodes and forwards those readings to Amazon AWS IoT Core using a Quectel FC41D Wi‑Fi/Bluetooth module.  It is based on the original ESP‑Now project from [ESP‑Now‑Sensor‑Network‑at‑the‑Edge‑with‑Gateway‑to‑AWS‑IoT](https://github.com/ghitabn/ESP-Now-Sensor-Network-at-the-Edge-with-Gateway-to-AWS-IoT), but the code and documentation have been updated to use the **Quectel FC41D** module (ordering code **FC41DAEMD**) instead of the ESP8266/ESP32 boards.  The FC41D is built around the BK7231MD chipset, provides Wi‑Fi 4 (2.4 GHz) and Bluetooth 5.2, and exposes a standard AT command set for network and MQTT functions.

## Why FC41D?

The FC41D module combines a 120 MHz ARM968 core, 256 KB of RAM and either 2 MB or 4 MB of flash into a very compact package (20 mm × 18 mm × 2.6 mm).  It supports IEEE 802.11 b/g/n (Wi‑Fi 4) and Bluetooth 5.2, and includes hardware security features such as WPA3‑SAE.  According to the official specification, FC41D features large memory, multiple GPIOs (UART/SPI/I²C/ADC/PWM) and an ultra‑compact form factor, while consuming little power.  Because the module integrates the radio, MCU and flash in a single low‑cost package and communicates over a simple UART interface using AT commands, it can replace both the ESP8266‑based sensor node and the separate Wi‑Fi transmitter used in the original project.  In many markets FC41D modules are sold at similar or lower cost than ESP8266 boards, yet they provide more memory, Bluetooth 5.2 support and an updated security stack (including WPA3).  This makes FC41D a cost‑effective alternative for large sensor networks where bill of materials (BOM) cost is important.

## Architecture

The original ESP‑Now architecture uses a dedicated low‑power ESP‑Now link to forward sensor readings from multiple ESP8266/ESP32 nodes to a gateway.  The gateway then forwards the data to AWS IoT using a second ESP32 device that connects over Wi‑Fi and publishes MQTT messages.  FC41D does not support Espressif’s proprietary ESP‑Now protocol, so the design is slightly modified:

* **Sensor nodes** are based on a low‑cost microcontroller (for example an Arduino UNO/Nano or STM32) connected via UART to a FC41D module.  Each node reads its sensor(s) and sends JSON‑formatted payloads to the FC41D using AT commands.  The module itself handles Wi‑Fi connectivity and MQTT publishing, so no separate “Wi‑Fi transmitter” board is required.
* A **gateway** is optional.  If several sensor nodes share one FC41D module, a gateway microcontroller can aggregate messages from multiple sensor nodes over a UART bus and forward them to a single FC41D module.  The gateway example provided here shows how to read binary sensor frames from a serial line, convert them into JSON and publish them to AWS IoT via AT commands.

The overall flow is therefore:

1. **Initialisation:** The FC41D module is configured via AT commands to load AWS IoT certificates, set up TLS and MQTT parameters and connect to the user’s Wi‑Fi network and AWS IoT endpoint.  Example AT commands for this are listed below and taken from the official AWS IoT Platform Access Guide.
2. **Sensor reading:** The microcontroller periodically reads sensor values (temperature, humidity, etc.) and formats the data into a simple JSON string.
3. **Publishing:** The microcontroller sends an `AT+QMTPUBRAW` command to FC41D along with the desired topic and payload length, then transmits the JSON payload.  FC41D handles the TLS/MQTT transport to AWS IoT.

This repository contains example source files illustrating both the sensor node and the gateway implementations.  They are written for the Arduino framework but can be adapted to any microcontroller with a UART.

## Advantages over ESP8266

Compared to the ESP8266 solution used in the original project, FC41D offers several advantages:

* **Lower BOM cost:** FC41D integrates the MCU, Wi‑Fi/Bluetooth radio and flash memory in a single module.  Because it can be controlled entirely via AT commands, the host microcontroller can be a very inexpensive 8‑bit MCU.  ESP8266‑based designs require either multiple ESP8266 boards (one for the sensor and one for Wi‑Fi transmission) or a more expensive ESP32 for AWS connectivity.
* **More memory and security features:** FC41D provides 256 KB RAM and 2 MB/4 MB flash.  ESP8266 modules typically have 50 KB of free RAM and 1 MB flash, which limits TLS and MQTT functionality.  FC41D supports WPA3‑SAE and can run TLS without external PSRAM.
* **Bluetooth 5.2:** In addition to Wi‑Fi 4, FC41D supports Bluetooth 5.2.  Future enhancements could allow sensor nodes to communicate with the gateway using BLE instead of Wi‑Fi.
* **Compact form factor:** The FC41D LCC package measures only 20 × 18 × 2.6 mm, enabling very small sensor designs.

## Repository contents

```
.
├── LICENSE                         – MIT licence from the original project
├── README.md                       – This document
├── diagrams/
│   └── basic_architecture.png      – Original architecture diagram for reference
├── node/
│   ├── include/
│   │   ├── datatypes.h             – Data type definitions (unchanged)
│   │   ├── node_config.h           – Node configuration for FC41D
│   │   └── fc41d.h                 – Helper functions for FC41D AT commands
│   ├── platformio.ini              – Example PlatformIO configuration for an Arduino host MCU
│   └── src/
│       └── main.cpp                – Sensor node example using FC41D
└── wifi_transmitter/
    ├── README.md                   – Notes on using FC41D as a gateway
    ├── include/
    │   ├── gateway_config.h        – Gateway configuration (Wi‑Fi, AWS endpoint, UART pins)
    │   └── fc41d_gateway.h         – Helper functions for AT commands (gateway variant)
    ├── src/
    │   ├── main.cpp                – Gateway example reading binary frames and publishing JSON
    │   └── fc41d_gateway.cpp       – Implementation of the gateway helper functions
    └── platformio.ini              – Build configuration for the gateway example
```

### node/include/datatypes.h

Defines simple numeric codes for different sensor payload types.  The file is copied unchanged from the original project.

### node/include/node_config.h

Configuration parameters for sensor nodes.  You can set the `NODE_ID`, Wi‑Fi SSID/password and AWS IoT endpoint here.  Unlike the original project there are no `ESP8266` or `ESP32` definitions – the host MCU communicates with the FC41D over UART and instructs it using AT commands.

### node/include/fc41d.h

Declares helper functions used by the sensor and gateway examples to initialise the FC41D module and publish MQTT messages via AT commands.

### node/src/main.cpp

Example Arduino sketch for a sensor node with a DHT11 temperature/humidity sensor.  The node reads the sensor every few seconds, builds a JSON payload and publishes it to AWS IoT via the FC41D module.  See the comments in the source file for details.

### wifi_transmitter/README.md and wifi_transmitter/src/main.cpp

Provide an optional gateway example.  If multiple sensor MCUs share a single FC41D module, the gateway example shows how to read binary frames from a serial bus (as in the original ESP‑Now project), convert them to JSON and publish them via AT commands.

## Configuring FC41D for AWS IoT

The FC41D module exposes a rich AT command set.  To connect to AWS IoT Core you need to load your certificates and configure TLS and MQTT parameters.  The following sequence, taken from the official AWS IoT platform access guide, illustrates the required steps:

```
// Write certificates and keys to the module’s file system (file indexes are examples)
AT+QSSLCERT="CA",2,1187
AT+QSSLCERT="User Cert",2,1220
AT+QSSLCERT="User Key",2,1675

// Configure SSL
AT+QSSLCFG="ciphersuite",1,0xFFFF
AT+QMTCFG="session",1,1
AT+QMTCFG="ssl",1,1,1
AT+QSSLCFG="sni",1,0
AT+QSSLCFG="verify",1,2

// Open an MQTT socket (replace endpoint and port with your AWS settings)
AT+QMTOPEN=1,"<your‑aws‑endpoint>.iot.<region>.amazonaws.com",8883

// Connect the MQTT client (replace "mything" with your Thing Name)
AT+QMTCONN=1,"mything"
```

After the connection is established you can publish messages using `AT+QMTPUBRAW`:

```
AT+QMTPUBRAW=<client>,<qos>,<retain>,<dup>,"<topic>",<length>
> {"state":{"reported":{"temperature":24.5}}}
```

See the source code in this repository for examples of sending these commands programmatically from an Arduino host MCU.  For detailed information refer to the **FC41D AT Commands Manual** and **AWS IoT Platform Access Guide** (both included in the `/Hardware FC41D.zip` and `/Software FC41D.zip` packages provided by the user).

## Licence

This project is released under the [MIT Licence](LICENSE), as in the original ESP‑Now project.
