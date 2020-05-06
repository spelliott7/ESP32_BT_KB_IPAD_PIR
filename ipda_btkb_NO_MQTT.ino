#include <WiFi.h>
#include <BleKeyboard.h>




//User selected properties 
const PROGMEM char* WIFI_SSID = "WiFi SSID";
const PROGMEM char* WIFI_PASSWORD = "WiFi Pass";

const PROGMEM char* PIN_CODE = "0000"; //iPad pin code

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
BleKeyboard bleKeyboard;

void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
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

  Serial.println("Ready to send PIN via BT");
  bleKeyboard.begin();
  pinMode(PIR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectsMovement, RISING);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
}

void loop() {

  now = millis();
  
  m_pir_value = digitalRead(PIR_PIN);
  if (m_pir_value == HIGH) {
    if (m_pir_state == LOW) {
      // a motion is detected
      Serial.println("INFO: Motion detected");
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
      m_pir_state = LOW;
      }
    }
  }
