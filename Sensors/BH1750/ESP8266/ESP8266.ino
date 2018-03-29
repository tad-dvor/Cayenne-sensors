/*
This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

Connection:

    VCC -> 3V3
    GND -> GND
    SCL -> SCL (GPIO5)
    SDA -> SDA (GPIO4)
    ADD -> (not connected) or GND
*/

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <Wire.h>
#include <BH1750.h>

// WiFi network info.
char ssid[] = "ssid";
char wifiPassword[] = "wifiPassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

unsigned long lastMillis = 0;

BH1750 lightMeter;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  Wire.begin(4, 5); // SDA, SCL
  lightMeter.begin();
}

void loop() {
	Cayenne.loop();

	//Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, lastMillis);

    uint16_t lux = lightMeter.readLightLevel();
    Cayenne.luxWrite(1, lux);
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
