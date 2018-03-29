// This example shows how to connect INA219 to Cayenne using an Ethernet W5500 shield and send/receive sample data.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTEthernetW5500.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

unsigned long lastMillis = 0;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float current_A = 0;
float loadvoltage = 0;
float power_W = 0;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
  uint32_t currentFrequency;
  ina219.begin(); 
}

void loop() {
	Cayenne.loop();
	//Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, lastMillis);

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    current_A = current_mA / 1000;
    power_W = current_A * loadvoltage; 
    Cayenne.virtualWrite(1, loadvoltage);
    Cayenne.virtualWrite(2, current_A);
    Cayenne.virtualWrite(3, power_W);
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
