# ESP32_BT_KB_IPAD_PIR
ESP32 MQTT enabled PIR with Bluetooth keyboard

I use an iPad in my garage to control Home Assistant, cast music, and score darts (of course). But to do that currently have to 
unlock the iPad, open HA, and turn on the switch for the dartboard light, turn on the Xbox and TV so I can play music / TV etc.  
As you can imagine that is tedious so I got a great idea from /u/RubberDinero to add a PIR and a bluetooth keyboard library
to an ESP32 so it could connect and unlock it. I use the MQTT message to fire up the other devices.  Voila.

This essentially sees motion > sends space bar > space bar (to wake the screen via BT keyboard) > waits a couple of seconds > sends four
digit code (did not test 6 but it should work) > then does nothing for 5 minutes until it can send again.  I included MQTT publishing so
you can base other triggers around it within HA / NodeRed / whatever.  Hoping I can figure out how to do a timed response with a run once
for the keyboard command so I can continue allowing the PIR to send data to MQTT but I don't know C++ well enough. Using the "delay"
function stops all other activity until the timer ends. But without that delay the keyboard would send the commands 'space, space, pin
code' every time it detected motion every few seconds. Probably don't want that.

At some point I'd like to build it as a custom component in Esphome so that it can be a fully functional device with the ability to send
the iPad unlock as a trigger in response to automation but can still include other pins for temp, OTA, etc. 

