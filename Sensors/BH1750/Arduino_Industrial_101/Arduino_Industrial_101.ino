/*
This example shows how to connect BH1750 to Cayenne using an Arduino Industrial 101 and send/receive sample data.

Connection:

    VCC -> VCC 
    GND -> GND
    SCL -> SCL (D0)
    SDA -> SDA (D1)
    ADD -> (not connected) or GND
*/

#include <Ciao.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

unsigned long lastMillis = 0;

void setup()
{
  Ciao.begin();  
  lightMeter.begin();                      
}

void loop()                     
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
    
    uint16_t lux = lightMeter.readLightLevel();
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(lux));
  }  
}
