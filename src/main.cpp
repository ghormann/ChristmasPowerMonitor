/*
 * This implements 8 amperage sensors that sen the amount of AMPS consumed for each
 * of the 8 curcits ever 1 second via MQTT.
*/
#include <Arduino.h>
#include "MyNetwork.h"
#include "MySensor.h"

MyNetwork network;
MySensor sensor;
long lastTs = 0; // used to tack when we last sampled

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  network.setup();
  sensor.setup();
  lastTs = millis();
}

void loop()
{
  // Call Everytime though loop to handle network traffic.
  network.loop();

  // This block runs every 500ms()
  // Note: sample() takes around 500ms() to return and 
  // the counter doesn't resent until it returns to give
  // mqtt time for sending message.
  if ((millis() - lastTs) > 500)
  {
    String result = sensor.sample();
    lastTs = millis();
    Serial.println(result);
    network.publish(result);
  }
}