/*
 * fc41d.h
 *
 * Helper functions for sending AT commands to a Quectel FC41D module from
 * a microcontroller host.  These functions are used by the example
 * sensor node and gateway sketches to initialise Wi‑Fi and MQTT
 * connections and publish messages to AWS IoT.  They are not
 * comprehensive; consult the FC41D AT Commands Manual for further
 * commands and options.
 */

#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

// Configure the FC41D module: load certificates, connect to Wi‑Fi,
// open a TLS/MQTT connection to AWS IoT and authenticate.  This
// function reads constants from node_config.h (Wi‑Fi credentials,
// endpoint, thing name).  It should be called once during setup.
void fc41d_init(SoftwareSerial &fc41d);

// Publish a JSON payload to the specified topic.  Returns true if the
// AT commands were sent successfully.  The client index and quality
// of service (QoS) are fixed in this example but can be changed
// through node_config.h.
bool fc41d_publish(SoftwareSerial &fc41d, const char *topic, const String &payload);

// Helper to send a single AT command followed by CRLF and wait for
// “OK” or “ERROR” response.  Returns true on OK.
bool fc41d_sendCommand(SoftwareSerial &fc41d, const String &cmd, unsigned long timeoutMs = 5000);