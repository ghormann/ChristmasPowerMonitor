
#ifndef My_Network_h_
#define My_Network_h_

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

class MyNetwork
{

public:
    void loop();
    void setup();
    void publish(String msg);

private:
    void mqtt_reconnect();
    void check_network();
};

#endif