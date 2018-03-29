// This example shows how to connect ML8511 to Cayenne using an Arduino Industrial 101 and send/receive sample data.

#include <Ciao.h>

unsigned long lastMillis = 0;

int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board

void setup()
{
  Ciao.begin();  
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);                  
}

void loop()                     
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
    
    int uvLevel = averageAnalogRead(UVOUT);
    int refLevel = averageAnalogRead(REF_3V3);
    float outputVoltage = 3.3 / refLevel * uvLevel;
    float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(uvIntensity));
  }  
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
