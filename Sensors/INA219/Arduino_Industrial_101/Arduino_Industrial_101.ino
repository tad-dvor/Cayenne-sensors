#include <Ciao.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float current_A = 0;
float loadvoltage = 0;
float power_W = 0;

unsigned long lastMillis = 0;

void setup()
{
  Ciao.begin();  
  uint32_t currentFrequency;
  ina219.begin();                 
}

void loop()                     
{ 
  //Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/0", String(lastMillis));
   
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    current_A = current_mA / 1000;
    power_W = current_A * loadvoltage; 
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/1", String(loadvoltage));
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/2", String(current_A));
    Ciao.write("mqtt","v1/MQTTusername/things/ClientID/data/3", String(power_W));
  }
}
