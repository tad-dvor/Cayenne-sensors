// This example shows how to connect TSL2561 to Cayenne using an Arduino Industrial 101 and send/receive sample data.

#include <Ciao.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

// Address used to read from the TSL2561. This is determined by the ADDR pin on the TSL2561.
// If ADDR is unconnected it means the sensor will use TSL2561_ADDR_FLOAT (0x39) for the address.  See the TSL2561 datasheet for more info.
const int address = TSL2561_ADDR_FLOAT;

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(address, 12345);

unsigned long lastMillis = 0;

void setup()
{
  Ciao.begin();  
  tsl.begin();
  
  tsl.enableAutoRange(true);
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */                     
}

void loop()                     
{
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
    
    sensors_event_t event;
    tsl.getEvent(&event);
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(event.light));
  }  
}
