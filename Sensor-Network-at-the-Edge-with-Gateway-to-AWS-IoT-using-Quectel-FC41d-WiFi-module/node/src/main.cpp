#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include "node_config.h"
#include "datatypes.h"
#include "fc41d.h"

// Structure matching the original project. Each reading contains a
// numeric node ID, a floating point value and a type code.
typedef struct dataReading {
  uint16_t id;
  float    data;
  uint8_t  type;
} dataReading;

// Global sensor reading
static dataReading currentData;

// Instantiate a DHT11 sensor. See node_config.h for the pin definition.
static DHT dht(DHT_PIN, DHT11);

// Create a software serial interface to the FC41D. The pins are
// defined in node_config.h. Ensure you are using a microcontroller
// that supports SoftwareSerial on these pins.
static SoftwareSerial fc41dSerial(FC41D_RX_PIN, FC41D_TX_PIN);

void setup() {
  // Start the hardware serial port for debugging
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial to be ready
  }
  Serial.println("FC41D sensor node starting...");

  // Initialise the DHT sensor
  dht.begin();

  // Initialise the software serial port to communicate with FC41D
  fc41dSerial.begin(115200);
  delay(100);

  // Configure the FC41D: load certificates, join Wi‑Fi, open MQTT
  fc41d_init(fc41dSerial);
}

static void publishMeasurement(float value, uint8_t typeCode) {
  currentData.id   = NODE_ID;
  currentData.data = value;
  currentData.type = typeCode;
  // Build a simple JSON payload
  String payload = "{\"id\":" + String(currentData.id) + ",\"type\":" + String(typeCode) + ",\"data\":" + String(value, 2) + "}";
  // Publish via AT command
  if (!fc41d_publish(fc41dSerial, AWS_TOPIC, payload)) {
    Serial.println("Failed to publish payload: " + payload);
  }
}

void loop() {
  // Read temperature
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    publishMeasurement(temp, TEMP_T);
    Serial.println("Published temperature: " + String(temp));
  }

  // Read humidity
  float hum = dht.readHumidity();
  if (!isnan(hum)) {
    publishMeasurement(hum, HUMIDITY_T);
    Serial.println("Published humidity: " + String(hum));
  }

  // Delay between readings
  delay(5000);
}