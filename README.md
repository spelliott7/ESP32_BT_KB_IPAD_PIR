# ESP32_BT_KB_IPAD_PIR
ESP32 MQTT enabled PIR with Bluetooth keyboard

I use an iPad in my garage to control Home Assistant, cast music, and score darts (of course). But to do that currently have to 
unlock the iPad, open HA, and turn on the switch for the dartboard light, turn on the Xbox and TV so I can play music / TV etc.  
As you can imagine that is tedious so I got a great idea from /u/RubberDinero to add a PIR and a bluetooth keyboard library
to an ESP32 so it could connect and unlock it. I use the MQTT message to fire up the other devices.  Voila.

I forgot to include the BleKeyboard library info: https://github.com/T-vK/ESP32-BLE-Keyboard
Download the zip file and upload it to the library in Arduino. Examples included. More info on that page.

This essentially sees motion > sends space bar > space bar (to wake the screen via BT keyboard) > waits a couple of seconds > sends four
digit code (did not test 6 but it should work) > then does nothing for 5 minutes until it can send again.  I included MQTT publishing so
you can base other triggers around it within HA / NodeRed / whatever.  Hoping I can figure out how to do a timed response with a run once
for the keyboard command so I can continue allowing the PIR to send data to MQTT but I don't know C++ well enough. Using the "delay"
function stops all other activity until the timer ends. But without that delay the keyboard would send the commands 'space, space, pin
code' every time it detected motion every few seconds. Probably don't want that.

At some point I'd like to build it as a custom component in Esphome so that it can be a fully functional device with the ability to send
the iPad unlock as a trigger in response to automation but can still include other pins for temp, OTA, etc. 

To configure: Set all the options for WiFi and MQTT server at the top.  Set your pin code inside of the quotes provided to whatever
you use. You should be able to put 6 in there.  If you want it to reset the trigger faster or slower or to match the timeout on your iPad
just adjust the "#define timeSeconds 300" to however long you want.  USE SECONDS, not minutes. At default it's set to 5 minutes between
triggers so you can adjust as needed. Set your Partition Scheme to No OTA 2MB / 2MB SPIFFS to make it fit. Apparently the bleKeyboard is a large library.

You can also set the pins to match whatever pins you like just change:
const int led = 26;
const PROGMEM uint8_t PIR_PIN = 27

The LED pin isn't required and is safe to ignore if you don't want an led to light up when unlocked. PIR is kind of necessary. 
Have fun!
