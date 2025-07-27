// Configuration file for sensor node using a Quectel FC41D module
//
// Set the node ID.  Each sensor node should have a unique ID within the network.
#define NODE_ID        1

// Wi‑Fi credentials.  These will be passed to the FC41D via AT commands.
// Replace with your SSID and password.
#define WIFI_SSID      "your_wifi_ssid"
#define WIFI_PASSWORD  "your_wifi_password"

// AWS IoT endpoint and thing name.  Replace with the endpoint assigned to your AWS account
// and the thing name you created in the AWS console.  Do not include the protocol prefix.
#define AWS_ENDPOINT   "a1example-ats.iot.us-east-1.amazonaws.com"
#define AWS_THINGNAME  "fc41d_sensor_1"

// MQTT topic to publish sensor data.  You can change this to suit your application.
#define AWS_TOPIC      "sensors/fc41d"

// Pin assignments for the FC41D module’s UART.  These values assume an Arduino UNO.
// The FC41D’s UART0 (default AT command interface) should be connected to the host MCU.
// RX_PIN is the Arduino’s receive pin (from FC41D TX), TX_PIN is transmit.
#define FC41D_RX_PIN   10
#define FC41D_TX_PIN   11

// DHT11 sensor pin definition.  If you are not using a DHT11 sensor you can
// remove the include and related code in main.cpp.
#define DHT_PIN        2

// AT command client index.  FC41D allows multiple MQTT clients; we use index 1.
#define MQTT_CLIENT    1