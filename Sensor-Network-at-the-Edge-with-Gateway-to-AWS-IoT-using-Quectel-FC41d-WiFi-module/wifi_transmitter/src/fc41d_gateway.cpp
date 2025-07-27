/*
 * fc41d_gateway.cpp
 *
 * Implementation of helper functions for controlling a Quectel FC41D
 * module via AT commands.  This variant is used by the gateway
 * example and reads configuration parameters from `gateway_config.h`.
 * The code connects to Wi‑Fi, configures TLS/MQTT settings and
 * publishes JSON payloads to AWS IoT.  It is deliberately simple
 * and does not implement full error handling or reconnection logic.
 */

#include "fc41d_gateway.h"
#include "gateway_config.h"

// Wait for a specific token from the FC41D serial stream.  Returns
// true if the token is found before the timeout.
static bool waitForResponse(SoftwareSerial &fc41d, const char *token, unsigned long timeoutMs) {
  unsigned long start = millis();
  String buffer;
  while (millis() - start < timeoutMs) {
    while (fc41d.available()) {
      char c = fc41d.read();
      buffer += c;
      if (buffer.endsWith(token)) {
        return true;
      }
    }
  }
  return false;
}

bool fc41d_sendCommand_gateway(SoftwareSerial &fc41d, const String &cmd, unsigned long timeoutMs) {
  fc41d.println(cmd);
  // Return true on OK
  if (waitForResponse(fc41d, "OK\r\n", timeoutMs)) {
    return true;
  }
  // Drain ERROR
  waitForResponse(fc41d, "ERROR\r\n", 100);
  return false;
}

void fc41d_init_gateway(SoftwareSerial &fc41d) {
  // Send basic commands to check the module and disable echo
  fc41d_sendCommand_gateway(fc41d, "AT", 1000);
  delay(100);
  fc41d_sendCommand_gateway(fc41d, "ATE0", 1000);
  fc41d_sendCommand_gateway(fc41d, "AT+RST", 5000);
  delay(2000);

  // Join Wi‑Fi network
  String cmd = String("AT+CWJAP=\"") + WIFI_SSID + "\",\"" + WIFI_PASSWORD + "\"";
  fc41d_sendCommand_gateway(fc41d, cmd, 15000);

  // TLS/MQTT configuration (ciphersuite, session, SSL etc.) – adapted
  // from the AWS IoT platform access guide【281473281446401†L495-L524】.
  fc41d_sendCommand_gateway(fc41d, "AT+QSSLCFG=\"ciphersuite\",1,0xFFFF", 5000);
  fc41d_sendCommand_gateway(fc41d, "AT+QMTCFG=\"session\",1,1", 5000);
  fc41d_sendCommand_gateway(fc41d, "AT+QMTCFG=\"ssl\",1,1,1", 5000);
  fc41d_sendCommand_gateway(fc41d, "AT+QSSLCFG=\"sni\",1,0", 5000);
  fc41d_sendCommand_gateway(fc41d, "AT+QSSLCFG=\"verify\",1,2", 5000);

  // Open MQTT socket and connect
  cmd = String("AT+QMTOPEN=") + MQTT_CLIENT + ",\"" + AWS_ENDPOINT + "\",8883";
  fc41d_sendCommand_gateway(fc41d, cmd, 30000);
  // Connect to AWS IoT with Thing Name
  cmd = String("AT+QMTCONN=") + MQTT_CLIENT + ",\"" + AWS_THINGNAME + "\"";
  fc41d_sendCommand_gateway(fc41d, cmd, 15000);
}

bool fc41d_publish_gateway(SoftwareSerial &fc41d, const char *topic, const String &payload) {
  // Publish header: AT+QMTPUBRAW=client,qos,retain,dup,"topic",length
  String header = String("AT+QMTPUBRAW=") + MQTT_CLIENT + ",0,0,0,\"" + String(topic) + "\"," + payload.length();
  if (!fc41d_sendCommand_gateway(fc41d, header, 5000)) {
    return false;
  }
  // Send payload and wait for OK
  fc41d.println(payload);
  return waitForResponse(fc41d, "OK\r\n", 10000);
}