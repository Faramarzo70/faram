#include "fc41d.h"
#include "node_config.h"

/*
 * Implementation of helper functions for FC41D AT command control.
 * These functions send AT commands through a SoftwareSerial instance and
 * wait for the module to respond with "OK".  For simplicity the
 * functions assume the FC41D is configured with echo disabled and
 * produces an OK/ERROR response for each command.  In production
 * applications you should implement more robust parsing and error
 * handling.
 */

// Read lines from the module until we see the expected result or timeout
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

bool fc41d_sendCommand(SoftwareSerial &fc41d, const String &cmd, unsigned long timeoutMs) {
  fc41d.println(cmd);
  // Wait for OK or ERROR
  if (waitForResponse(fc41d, "OK\r\n", timeoutMs)) {
    return true;
  }
  // Check for ERROR
  waitForResponse(fc41d, "ERROR\r\n", 100);
  return false;
}

void fc41d_init(SoftwareSerial &fc41d) {
  // Basic module reset (optional)
  fc41d_sendCommand(fc41d, "AT", 1000);
  delay(100);
  fc41d_sendCommand(fc41d, "ATE0", 1000); // disable echo
  fc41d_sendCommand(fc41d, "AT+RST", 5000);
  delay(2000);

  // Join Wi‑Fi network
  String cmd = "AT+CWJAP=\"" + String(WIFI_SSID) + "\",\"" + String(WIFI_PASSWORD) + "\"";
  fc41d_sendCommand(fc41d, cmd, 15000);

  // MQTT SSL configuration.  The following commands mirror the AWS IoT access guide【281473281446401†L495-L524】.
  // Note: The file indexes (2,1187 etc.) must be adapted to match where you store
  // certificates on the module’s file system.  See the FC41D AT commands manual.
  fc41d_sendCommand(fc41d, "AT+QSSLCFG=\"ciphersuite\",1,0xFFFF", 5000);
  fc41d_sendCommand(fc41d, "AT+QMTCFG=\"session\",1,1", 5000);
  fc41d_sendCommand(fc41d, "AT+QMTCFG=\"ssl\",1,1,1", 5000);
  fc41d_sendCommand(fc41d, "AT+QSSLCFG=\"sni\",1,0", 5000);
  fc41d_sendCommand(fc41d, "AT+QSSLCFG=\"verify\",1,2", 5000);

  // Open MQTT socket
  cmd = "AT+QMTOPEN=" + String(MQTT_CLIENT) + ",\"" + String(AWS_ENDPOINT) + "\",8883";
  fc41d_sendCommand(fc41d, cmd, 30000);

  // Connect MQTT client.  Use thing name from config
  cmd = "AT+QMTCONN=" + String(MQTT_CLIENT) + ",\"" + String(AWS_THINGNAME) + "\"";
  fc41d_sendCommand(fc41d, cmd, 15000);
}

bool fc41d_publish(SoftwareSerial &fc41d, const char *topic, const String &payload) {
  // Send publish header.  Format: AT+QMTPUBRAW=client,qos,retain,dup,"topic",length
  String header = String("AT+QMTPUBRAW=") + MQTT_CLIENT + ",0,0,0,\"" + String(topic) + "\"," + payload.length();
  if (!fc41d_sendCommand(fc41d, header, 5000)) {
    return false;
  }
  // Send the payload followed by CRLF.  According to the manual the module will return OK when the message is sent.
  fc41d.println(payload);
  return waitForResponse(fc41d, "OK\r\n", 10000);
}