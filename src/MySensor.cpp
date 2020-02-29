#include "MySensor.h"

/**
 * Initialize the sensor.  
 */
void MySensor::setup()
{
    pins[0] = A0;
    pins[1] = A1;
    pins[2] = A2;
    pins[3] = A3;
    pins[4] = A4;
    pins[5] = A5;
    pins[6] = A6;
    pins[7] = A7;
    pins[8] = A8;
    /*
    pins[9] = A9;
    pins[10] = A10;
    pins[11] = A11;
    pins[12] = A12;
    pins[13] = A13;
    pins[14] = A14;
    pins[15] = A15;
    */

    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        sampleSum[i] = 0;
    }
}
 
/**
 * Called within loop to take a sample.
 * The samping process takes 500ms. 
 * Result is a json array of the measurements
 */
String MySensor::sample()
{

    unsigned long lastTs = millis();
    int sampleCount = 0;

    // One would think that 480 (mulitple of 60hz) would be best, but I found this would 
    // report too low when compared to a Kil-A-Watt. 
    // 500ms gives results much closer to measured actuals. 
    while (sampleCount < 500) 
    {
        // Read the ssensor once per ms.
        if ((millis() - lastTs) >= 1)
        { // 1 ms samples
            lastTs = millis();
            for (int i = 0; i < SENSOR_COUNT; i++)
            {
                long before = sampleSum[i];
                // Must cast to long before squaring
                long temp = analogRead(pins[i]) - 512;
                sampleSum[i] += sq(temp);
                if ( sampleSum[i] < before) {
                    Serial.println("DEBUG: Overflow : " + String(before) + ", " + String(sampleSum[i]) + " i = " + String(i) + " cnt=" + String(sampleCount));
                }
            }
            ++sampleCount;
        }
    }

    String answer = "[";
    for (int i = 0; i < SENSOR_COUNT; i++)
    {

        float mean = sampleSum[i] / sampleCount;
        float rms = sqrt(mean);
        float amps = (rms * VPC) / SENSATIVITY;
        sampleSum[i] = 0; // Reset
        if (isnan(amps)) {
            amps = -1.0;
        }
        answer.concat(amps);
        if (i < (SENSOR_COUNT - 1))
        {
            answer.concat(",");
        }
    }
    answer.concat("]");
    return answer;
}