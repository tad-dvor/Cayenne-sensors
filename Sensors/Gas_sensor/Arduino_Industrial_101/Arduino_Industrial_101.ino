#include <Ciao.h>

unsigned long lastMillis = 0;

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
   
    int analogValue = analogRead(A0);
    int gas = map(analogValue, 0, 1023, 0, 100);
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(gas));
  }
}
