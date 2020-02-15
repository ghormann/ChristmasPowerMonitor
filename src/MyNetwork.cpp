#include <Arduino.h>
#include "MyNetwork.h"

EthernetClient ethClient;
PubSubClient client(ethClient);

// Update these with values suitable for your network.
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};
IPAddress mqtt_server(192, 168, 0, 212);
const char * device = "Power1";
const char *topic = "/christmas/power/Power1"; 

void mqtt_setup()
{
    Serial.println("Starting eithernet");
    Ethernet.begin(mac);
    client.setServer(mqtt_server, 1883);
    // Allow the hardware to sort itself out
    delay(1500);
    mqtt_reconnect();
}

// This must be called inside the loop to keep conneciton alive.
void mqtt_loop()
{
    check_network();
    if (!client.connected())
    {
        mqtt_reconnect();
    }
    client.loop();
    client.publish(topic, "hello world");
}

void check_network() {
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

void mqtt_reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("");
        Serial.print("IP = ");
        Serial.println(Ethernet.localIP());
        Serial.println("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(device))
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
