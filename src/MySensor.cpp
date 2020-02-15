#include "MySensor.h"

void MySensor::sample()
{

    Serial.println("Starting Sample");
    unsigned long lastTs = millis();
    int sampleCount = 0;
    int sampleSum = 0;

    while (sampleCount < 1000)
    {
        // Read the sensor when needed.
        if ((millis() - lastTs) >= 1)
        { // 1 ms samples
            lastTs = millis();
            sampleSum += sq(analogRead(A0) - 512);
            ++sampleCount;
        }
    }

    float mean = sampleSum / sampleCount;
    float rms = sqrt(mean);
    float amps = (rms * VPC) / SENSATIVITY;
    Serial.println("AMPS: " + String(amps));

    sampleCount = 0;
    sampleSum = 0;
}