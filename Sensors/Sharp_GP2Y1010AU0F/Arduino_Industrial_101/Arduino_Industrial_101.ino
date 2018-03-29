#include <Ciao.h>

unsigned long lastMillis = 0;

int measuringPin = A0;
int ledPin = 5;
const int timeMeasuring = 280;
const int timeStabilization = 40;
const int timeSleep = 9680;
float voltageAnalog = 0;
float voltageConversion = 0;
float dustiness = 0;

void setup()
{
  Ciao.begin();   
  pinMode(ledPin, OUTPUT);                
}

void loop()                     
{ 
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
   
    digitalWrite(ledPin, LOW);
    delayMicroseconds(timeMeasuring);
    voltageAnalog = analogRead(measuringPin);
    delayMicroseconds(timeStabilization);
    digitalWrite(ledPin, HIGH);
    delayMicroseconds(timeSleep);
    voltageConversion = voltageAnalog * (4.7 / 1024.0);  //Voltage on 5V pin on Arduino, My voltage is 4.7 V
    dustiness = (0.17 * voltageConversion - 0.1)*1000;
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(dustiness));
  }
}
