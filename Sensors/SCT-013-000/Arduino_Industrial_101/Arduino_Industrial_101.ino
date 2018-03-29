// Please watch the video. Wiring instructions: https://www.hackster.io/ShawnHymel/hack-your-home-part-3-power-monitor-16a313

#include <Ciao.h>
#include "math.h"

unsigned long lastMillis = 0;

// RMS voltage
const double vRMS = 230.0;      // Assumed or measured
// Parameters for measuring RMS current
const double offset = 2.265;     // Half the ADC max voltage ----- offset = pin5v / 2 
const int numTurns = 2000;      // 1:2000 transformer turns
const int rBurden = 267;        // Burden resistor value
const int numSamples = 1000;    // Number of samples before calculating RMS
float pin5v = 4.53;             //Voltage on 5V pin on Arduino, My voltage is 4.53V

void setup()
{
  Ciao.begin();                  
}

void loop()                     
{ 
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
   
    int sample;
    double voltage;
    double iPrimary;
    double acc = 0;
    double iRMS;
    double apparentPower;
    // Take a number of samples and calculate RMS current
    for ( int i = 0; i < numSamples; i++ ) {
        // Read ADC, convert to voltage, remove offset
        sample = analogRead(A0);
        voltage = (sample * pin5v) / 1024;
        voltage = voltage - offset;       
        // Calculate the sensed current
        iPrimary = (voltage / rBurden) * numTurns;
        // Square current and add to accumulator
        acc += pow(iPrimary, 2);
    }
    // Calculate RMS from accumulated values
    iRMS = sqrt(acc / numSamples);
    // Calculate apparent power
    apparentPower = vRMS * iRMS;
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(iRMS));
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/2", String(apparentPower));
  }
}
