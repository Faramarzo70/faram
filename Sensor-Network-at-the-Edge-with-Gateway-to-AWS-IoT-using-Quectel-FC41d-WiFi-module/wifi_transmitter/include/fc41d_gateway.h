/*
 * fc41d_gateway.h
 *
 * Helper functions for sending AT commands to a Quectel FC41D module
 * from a gateway microcontroller.  These functions are similar to
 * those used in the sensor node example but read constants from
 * `gateway_config.h`.  They send AT commands to configure Wi‑Fi,
 * MQTT and TLS settings and to publish messages to AWS IoT.  The
 * implementation is in `fc41d_gateway.cpp`.
 */

#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

// Initialise the FC41D module: disable echo, join the Wi‑Fi network,
// configure TLS/MQTT and connect to AWS IoT.  Reads settings from
// gateway_config.h.  Should be called once during setup.
void fc41d_init_gateway(SoftwareSerial &fc41d);

// Publish a JSON payload to the topic defined in gateway_config.h.  The
// `topic` parameter allows overriding the default topic.  Returns
// true on success.
bool fc41d_publish_gateway(SoftwareSerial &fc41d, const char *topic, const String &payload);

// Send a single AT command and wait for an OK response.  Returns
// true if the command succeeded.  Exposed for debugging.
bool fc41d_sendCommand_gateway(SoftwareSerial &fc41d, const String &cmd, unsigned long timeoutMs = 5000);