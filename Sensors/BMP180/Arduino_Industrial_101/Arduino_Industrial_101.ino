// This example shows how to connect BMP180 to Cayenne using an Arduino Industrial 101 and send/receive sample data.

#include <Ciao.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10180);

unsigned long lastMillis = 0;

void setup()
{
  Ciao.begin();  
  bmp.begin();                   
}

void loop()                     
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
    
    sensors_event_t event;
    bmp.getEvent(&event);
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(event.pressure));
    float temperature;
    bmp.getTemperature(&temperature);
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/2", String(temperature));
  }  
}
