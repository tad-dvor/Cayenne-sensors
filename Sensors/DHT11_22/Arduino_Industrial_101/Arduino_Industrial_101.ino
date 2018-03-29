#include <Ciao.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 6
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

unsigned long lastMillis = 0;

void setup()
{
  Ciao.begin();
  dht.begin();
  sensor_t sensor;                    
}

void loop()                     
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
    
    sensors_event_t event;  
    dht.temperature().getEvent(&event);
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(event.temperature));
    dht.humidity().getEvent(&event);
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/2", String(event.relative_humidity));
  }    
}
