// This example shows how to connect Gas sensor to Cayenne using an Ethernet W5200 shield and send/receive sample data.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTEthernetW5200.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

unsigned long lastMillis = 0;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop() {
	Cayenne.loop();
	//Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, lastMillis);

    int analogValue = analogRead(A0);
    int gas = map(analogValue, 0, 1023, 0, 100);
    Cayenne.virtualWrite(1, gas, "analog_sensor", "null");
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
