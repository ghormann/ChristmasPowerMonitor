# Christmas Power Monitor
An Aduino based project for measuring AC power consumption 
and posting the results an MQTT broker. It samples up to 16 sensors every 1 second and publishes on the MQTT bus.

## Required Hardware
* [ARDUINO MEGA 2560](https://store.arduino.cc/usa/mega-2560-r3)  -- Could be easily modifed to work with an Uno
* [ACS712 20A Current Sensors](https://www.amazon.com/gp/product/B079P7HLYK) - It is possible to use the 5A or 30A, you just need to change some constants
* [Arduino Eithernet Shield](https://store.arduino.cc/usa/arduino-ethernet-shield-2) - Should work with either a V1 (based on W5100) or V2 (based on W5500)

## Configuration 
The following changes need to be made before you compile
1. Modify include/MySensor.h if you changed the number and types of sensors 
1. Modify src/MyNetwork.cpp to chagne the Mac address, MQTT IP Address, MQTT Port, and MQTT Topic, 
1. Modify src/MySensor.cpp to change the pins (if desired) 
1. Compile code with Platform.io and Visual Studo Code

## TODO
* Move the Network config to main.cpp