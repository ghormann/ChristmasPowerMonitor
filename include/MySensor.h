
#ifndef My_Sensor_h_
#define My_Sensor_h_

#include <Arduino.h>

#define VPC 4.8828125  // 5 volts / 1024 divisions * 1000 (vols->millivolts)
#define SENSATIVITY 100 // From data sheet: 5A = 185 20A = 100, 30A = 66.  I'm using 20A.

class MySensor
{
public:
    void sample();
};

#endif