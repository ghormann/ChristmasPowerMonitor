#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

void mqtt_reconnect();
void mqtt_loop();
void mqtt_setup();
void check_network();