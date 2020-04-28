#include <WiFi.h>
#include <PubSubClient.h>
#include <BleKeyboard.h>




//User selected properties 
const PROGMEM char* WIFI_SSID = "WiFi SSID";
const PROGMEM char* WIFI_PASSWORD = "WiFi Pass";

// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "ipad_kb";
const PROGMEM char* MQTT_SERVER_IP = "MQTT IP/Server info";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "MQTT User";
const PROGMEM char* MQTT_PASSWORD = "MQTT Pass";
const PROGMEM char* PIN_CODE = "0000"; //iPad pin code
const PROGMEM char* MQTT_MOTION_STATUS_TOPIC = "ipad/motion"; // MQTT topic
#define timeSeconds 300 //set to number of seconds between activating BT keyboard commands

const PROGMEM char* MOTION_ON = "ON";
const PROGMEM char* MOTION_OFF = "OFF";


const PROGMEM uint8_t PIR_PIN = 27; //ESP32 PIR pin 
uint8_t m_pir_state = LOW; 
uint8_t m_pir_value = 0;
const int led = 26;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
BleKeyboard bleKeyboard;

void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void publishPirSensorState() {
  if (m_pir_state) {
    client.publish(MQTT_MOTION_STATUS_TOPIC, MOTION_OFF, true);
  } else {
    client.publish(MQTT_MOTION_STATUS_TOPIC, MOTION_ON, true);
  }
}

  
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("INFO: connected");
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // init the serial
  Serial.begin(115200);

  // init the WiFi connection
  Serial.println();
  Serial.println();
  Serial.print("INFO: Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("INFO: WiFi connected");
  Serial.println("INFO: IP address: ");
  Serial.println(WiFi.localIP());

  // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);

  Serial.println("Ready to send PIN via BT");
  bleKeyboard.begin();
  pinMode(PIR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectsMovement, RISING);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  now = millis();
  
  m_pir_value = digitalRead(PIR_PIN);
  if (m_pir_value == HIGH) {
    if (m_pir_state == LOW) {
      // a motion is detected
      Serial.println("INFO: Motion detected");
      publishPirSensorState();
      Serial.println("Sending wake command...");
      bleKeyboard.write(32);
      delay(100);
      bleKeyboard.write(32);
      delay(3500);
      bleKeyboard.print(PIN_CODE); 
      bleKeyboard.releaseAll();
      m_pir_state = HIGH;
      delay (timeSeconds*1000);
    }
  } else {
    if (m_pir_state == HIGH) {
      publishPirSensorState();
      m_pir_state = LOW;
      }
    }
  }
