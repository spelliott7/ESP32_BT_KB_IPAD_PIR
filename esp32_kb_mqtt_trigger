
#include <WiFi.h>
#include <PubSubClient.h>
#define USE_NIMBLE
#include <BleKeyboard.h>


void callback(char* topic, byte* payload, unsigned int length);

//EDIT THESE LINES TO MATCH YOUR SETUP
#define MQTT_SERVER "222.222.222.222"  //your MQTT IP Address
const PROGMEM uint16_t mqtt_port = 1883;
const char* ssid = "wifi ssid";
const char* password = "wifi pass";
const char* mqtt_client = "ipad_kb"; //whatever you want the client to be, must not match other clients in HA
const char* mqtt_user = "mqtt user";
const char* mqtt_pass = "mqtt pass";
const char* pin_code = "XXXX"; //iPad pin code
#define timeSeconds 300 //set to number of seconds between activating BT keyboard commands

char const* switchTopic = "ipad/kb";

WiFiClient wifiClient;
BleKeyboard bleKeyboard;
PubSubClient client(wifiClient);

void setup() {
  //start the serial line for debugging
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  client.setServer(MQTT_SERVER, mqtt_port);
  client.setCallback(callback);
   Serial.println("MQTT Connection Successful");
  Serial.println("Ready to send PIN via BT");
  bleKeyboard.begin();

  //wait a bit before starting the main loop
      delay(2000);
}
}

void loop(){

  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3) {reconnect();}

  //maintain MQTT connection
  client.loop();

 
}

void callback(char* topic, byte* payload, unsigned int length) {

  //convert topic to string to make it easier to work with
  String topicStr = topic; 

  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);

   if (topicStr == "ipad/kb") 
    {

     //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
     if(payload[0] == '1'){
        Serial.println("Unlocking iPad");
        bleKeyboard.write(32); // space to wake ipad. ascii for space = 32
        delay(1000);
        bleKeyboard.write(32); // the double-tap
        delay(4000); // 1000 = 1s
        bleKeyboard.println(pin_code); //sends code
        bleKeyboard.releaseAll(); // releases keys
        delay (timeSeconds);
       client.publish("ipad/kb/stat", "on");
       }

      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
     else if (payload[0] == '0'){
       Serial.println("Lock the iPad");
        bleKeyboard.press(KEY_LEFT_CTRL); // CTRL+COMMAND(WIN GUI)+q locks iPad
        delay(100);
        bleKeyboard.press(KEY_LEFT_GUI);
        delay(100);
        bleKeyboard.press(113);
        delay(100);
        bleKeyboard.releaseAll();
       client.publish("ipad/kb/stat", "off");
       }
     }

  }


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client, mqtt_user, mqtt_pass)) {
      Serial.println("INFO: connected");
      client.subscribe(switchTopic);
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}




