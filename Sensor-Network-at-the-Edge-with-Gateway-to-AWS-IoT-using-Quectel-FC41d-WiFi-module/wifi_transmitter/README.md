# Gateway example for FC41D

This directory contains an optional gateway example.  In the original ESP‑Now project, a separate *Wi‑Fi transmitter* device received binary frames from ESP‑Now sensor nodes and then forwarded those frames to AWS IoT using an ESP32.  With the Quectel FC41D, you can simplify the architecture: a single module can handle both Wi‑Fi and MQTT, and the host microcontroller can be very inexpensive.

The gateway example demonstrates how to aggregate sensor readings from multiple microcontrollers over a UART bus and publish them to AWS IoT Core.  Each sensor node sends a compact `dataReading` structure containing an ID, value and type (see `node/include/datatypes.h`).  The gateway reads these binary frames from a serial port, converts them into a JSON document using the ArduinoJson library and publishes them to AWS via the FC41D module.

## Files

* `include/gateway_config.h` – configuration header for Wi‑Fi credentials, AWS endpoint, UART pins and topic names.
* `include/fc41d_gateway.h` – helper functions for initialising the FC41D and publishing messages via AT commands.  These functions are similar to those used in the sensor node but read constants from `gateway_config.h`.
* `src/fc41d_gateway.cpp` – implementation of the helper functions.
* `src/main.cpp` – the gateway sketch.  Reads `dataReading` frames from `SENSOR_RX_PIN`/`SENSOR_TX_PIN`, converts them to JSON and publishes to AWS.
* `platformio.ini` – PlatformIO configuration for building the gateway example.  By default it targets an Arduino Mega 2560 (which has multiple hardware UARTs) and depends on the ArduinoJson library.

## Usage

1.  Connect a FC41D module to your gateway microcontroller via UART.  Define the pins used for TX/RX in `gateway_config.h` (`FC41D_RX_PIN` and `FC41D_TX_PIN`).  Make sure the baud rate in `main.cpp` matches the module’s default (115200 bps).
2.  Connect your sensor nodes to another UART interface (`SENSOR_RX_PIN`/`SENSOR_TX_PIN`) and send `dataReading` structures at the same baud rate.  In the example we assume each frame is sent without framing characters; the gateway simply reads the fixed number of bytes corresponding to the structure and then processes it.
3.  Edit `gateway_config.h` to set your Wi‑Fi SSID/password, AWS IoT endpoint, Thing Name and MQTT topic.  You must also load certificates into the FC41D as described in the root README.  The helper functions will send AT commands to join the network and open an MQTT connection.
4.  Build and flash the sketch using PlatformIO or the Arduino IDE.  When the gateway receives data from a node it will print debug information to `Serial` and publish the JSON payload via the FC41D.

This example is intentionally minimal and intended to be customised for your hardware.  For a production system you should add more robust error handling, buffering and reconnection logic.