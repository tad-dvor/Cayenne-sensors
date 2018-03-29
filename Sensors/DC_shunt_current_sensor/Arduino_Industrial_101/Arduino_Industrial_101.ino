#include <Ciao.h>

unsigned long lastMillis = 0;

float pin5v = 4.73;    //Voltage on 5V pin on Arduino, My voltage is 4.73V
float shuntR = 0.075;  //Shunt resistor 0.075 ohm
float shuntU;  //Voltage on shunt resistor
float current;

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
    shuntU = (pin5v / 1024) * analogValue;
    current = shuntU / shuntR;
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(current));
  }
}
