// This example shows how to connect HTU21D to Cayenne using an Arduino Industrial 101 and send/receive sample data.

#include <Ciao.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

unsigned long lastMillis = 0;

void setup()
{
  Ciao.begin();  
  htu.begin();                     
}

void loop()                     
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
    
    
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(htu.readTemperature()));
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/2", String(htu.readHumidity()));
  }  
}
