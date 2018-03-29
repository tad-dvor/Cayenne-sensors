// This example shows how to connect Sharp GP2Y1010AU0F to Cayenne using an Ethernet W5100 shield and send/receive sample data.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

unsigned long lastMillis = 0;

int measuringPin = A0;
int ledPin = 2;
const int timeMeasuring = 280;
const int timeStabilization = 40;
const int timeSleep = 9680;
float voltageAnalog = 0;
float voltageConversion = 0;
float dustiness = 0;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
  pinMode(ledPin, OUTPUT);
}

void loop() {
	Cayenne.loop();
	//Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
	if(millis() - lastMillis > 1000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, lastMillis);

    digitalWrite(ledPin, LOW);
    delayMicroseconds(timeMeasuring);
    voltageAnalog = analogRead(measuringPin);
    delayMicroseconds(timeStabilization);
    digitalWrite(ledPin, HIGH);
    delayMicroseconds(timeSleep);
    voltageConversion = voltageAnalog * (4.7 / 1024.0);  //Voltage on 5V pin on Arduino, My voltage is 4.7 V
    dustiness = (0.17 * voltageConversion - 0.1)*1000;
		Cayenne.virtualWrite(1, dustiness, "pm", "mg");
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
