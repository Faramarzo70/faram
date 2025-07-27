#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include "gateway_config.h"
#include "fc41d_gateway.h"

// Define the data structure sent by sensor nodes.  Each frame
// contains a node ID, a floating‑point data value and a type code.
typedef struct dataReading {
  uint16_t id;
  float    data;
  uint8_t  type;
} dataReading;

// SoftwareSerial ports for the FC41D module and the sensor bus.  If
// your board has multiple hardware serial ports (e.g. Arduino Mega),
// you may replace SoftwareSerial with Serial1/Serial2 etc.  Make
// sure the pins defined in gateway_config.h support SoftwareSerial.
SoftwareSerial fcSerial(FC41D_RX_PIN, FC41D_TX_PIN);
SoftwareSerial sensorSerial(SENSOR_RX_PIN, SENSOR_TX_PIN);

void setup() {
  // Debug console
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for USB serial
  }
  Serial.println("FC41D gateway starting...");
  // Initialise serial links
  fcSerial.begin(115200);
  sensorSerial.begin(115200);

  // Initialise the FC41D module
  Serial.println("Initialising FC41D...");
  fc41d_init_gateway(fcSerial);
  Serial.println("FC41D initialised.");
}

void loop() {
  // Read a dataReading frame from the sensor bus.  The gateway
  // expects the sensor node to send exactly sizeof(dataReading)
  // bytes per reading without additional framing characters.
  if (sensorSerial.available() >= sizeof(dataReading)) {
    dataReading reading;
    int received = sensorSerial.readBytes((char *)&reading, sizeof(reading));
    if (received == sizeof(reading)) {
      // Build JSON
      StaticJsonDocument<128> doc;
      doc["id"] = reading.id;
      doc["data"] = reading.data;
      doc["type"] = reading.type;
      char json[128];
      size_t len = serializeJson(doc, json);
      (void)len; // unused
      Serial.print("Publishing: ");
      Serial.println(json);
      // Publish to AWS
      if (!fc41d_publish_gateway(fcSerial, AWS_TOPIC, String(json))) {
        Serial.println("Publish failed");
      }
    }
  }
  // Yield to allow background processing
  delay(50);
}