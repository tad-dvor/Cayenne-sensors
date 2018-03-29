// This example shows how to connect DHT12 to Cayenne using an Arduino Yun and send/receive sample data.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTYun.h>
#include <Wire.h>
#include <dht.h>

dht12 DHT(0x5c);

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
  DHT.read();

	//Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 10000) {
		lastMillis = millis();
    // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
    Cayenne.virtualWrite(0, millis());
  
    float temperature = DHT.getTemperature() / 10.0;
    float humidity = DHT.getHumidity() / 10.0;
    float dewPoint = DHT.dewPoint();
  
    Cayenne.celsiusWrite(1, temperature);
    Cayenne.virtualWrite(2, humidity, "rel_hum", "p");
    Cayenne.virtualWrite(3, dewPoint);
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
