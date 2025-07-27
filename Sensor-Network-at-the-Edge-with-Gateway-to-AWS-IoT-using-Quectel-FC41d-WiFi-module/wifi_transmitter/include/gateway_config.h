/*
 * gateway_config.h
 *
 * Configuration parameters for the FC41D gateway example.  Edit the
 * definitions below to match your Wi‑Fi network, AWS IoT endpoint and
 * certificate settings.  See the root README for instructions on
 * loading certificates and connecting the FC41D module.
 */

#pragma once

#include <Arduino.h>

// Wi‑Fi credentials
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// AWS IoT Core
// Replace with your AWS endpoint without protocol or port (e.g.
// "abcd1234567890-ats.iot.eu-west-1.amazonaws.com").
#define AWS_ENDPOINT    "your‑endpoint.iot.region.amazonaws.com"

// Thing Name used when connecting to AWS IoT
#define AWS_THINGNAME   "fc41d_gateway"

// MQTT client ID used with AT+QMTOPEN and AT+QMTCONN.  The FC41D
// supports multiple concurrent clients; client index 1 is used here.
#define MQTT_CLIENT     1

// Topic for publishing aggregated sensor data.  Modify as needed.
#define AWS_TOPIC       "sensors/gateway"

// UART pin assignments.  The gateway communicates with the FC41D
// through a SoftwareSerial or hardware serial port defined by
// FC41D_RX_PIN/FC41D_TX_PIN.  It also reads incoming sensor frames
// from SENSOR_RX_PIN/SENSOR_TX_PIN.  Adjust these according to your
// hardware design.
#define FC41D_RX_PIN    10
#define FC41D_TX_PIN    11

#define SENSOR_RX_PIN   8
#define SENSOR_TX_PIN   9
