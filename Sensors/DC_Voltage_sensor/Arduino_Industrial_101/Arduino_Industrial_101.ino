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
   
    int analogValue = analogRead(A0); // read the input on analog pin 0
    float a = 5; //I am measuring 25V on sensor, 25V/5V=5
    float b = 0.00455522971; //Voltage on 5V pin on Arduino / 1024, My voltage is 4.66V > 4.66/1024=0.00455522971
    float voltage;
    voltage = analogValue * a * b;
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(voltage));
  }
}
