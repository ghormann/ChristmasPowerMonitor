#include <Arduino.h>
#include "MyNetwork.h"
#include "MyNetwork_private.h"

EthernetClient ethClient;
PubSubClient client(ethClient);

// ---------------------------------------------------------
// Update these with values suitable for your network.
#define MQTT_PORT 1883
byte mac[] = MAC;
IPAddress mqtt_server;
const char * device = MQTT_DEVICE;
const char * username = MQTT_USERNAME;
const char * password = MQTT_PASSWORD;
const char *topic = MQTT_TOPIC;
// ---------------------------------------------------------

void MyNetwork::setup()
{
    mqtt_server.fromString(MQTT_IP_ADDR);
    Serial.println("Starting Network");
    Ethernet.begin(mac);
    client.setServer(mqtt_server, MQTT_PORT);
    // Allow the hardware to sort itself out
    delay(1500);
    mqtt_reconnect();
}

// This must be called inside the loop to keep conneciton alive.
void MyNetwork::loop()
{
    check_network();
    if (!client.connected())
    {
        mqtt_reconnect();
    }
    client.loop();
}

/**
 * Method for publishing a message to MQTT
 */
void MyNetwork::publish(String msg) {
  client.publish(topic,msg.c_str());
}

/**
 * Private method for newing DHCP lease
 */
void MyNetwork::check_network() {
    switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error DHCP: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("DHCP Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error DHCP: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("DHCP Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }
}

/**
 * Private method for restoring  MQTT
 * if it goes down
 */
void MyNetwork::mqtt_reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("");
        Serial.print("IP = ");
        Serial.println(Ethernet.localIP());
        Serial.print("Attempting MQTT connection at: " );
        Serial.println(MQTT_IP_ADDR);
        // Attempt to connect
        if (client.connect(device, username, password))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.println(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
