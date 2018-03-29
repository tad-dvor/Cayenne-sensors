// This example shows how to connect SCT-013-000 to Cayenne using an Arduino/Genuino MKR1000 and send/receive sample data.
// Please watch the video. Wiring instructions: https://www.hackster.io/ShawnHymel/hack-your-home-part-3-power-monitor-16a313

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTMKR1000.h>
#include "math.h"

// WiFi network info.
char ssid[] = "ssid";
char wifiPassword[] = "wifiPassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

unsigned long lastMillis = 0;

// RMS voltage
const double vRMS = 230.0;      // Assumed or measured
// Parameters for measuring RMS current
const double offset = 1.65;     // Half the ADC max voltage
const int numTurns = 2000;      // 1:2000 transformer turns
const int rBurden = 267;        // Burden resistor value
const int numSamples = 1000;    // Number of samples before calculating RMS

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();
	//Publish data every 1 seconds (10000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, lastMillis);

    int sample;
    double voltage;
    double iPrimary;
    double acc = 0;
    double iRMS;
    double apparentPower;
    // Take a number of samples and calculate RMS current
    for ( int i = 0; i < numSamples; i++ ) {
        // Read ADC, convert to voltage, remove offset
        sample = analogRead(A0);
        voltage = (sample * 3.3) / 1024;
        voltage = voltage - offset;       
        // Calculate the sensed current
        iPrimary = (voltage / rBurden) * numTurns;
        // Square current and add to accumulator
        acc += pow(iPrimary, 2);
    }
    // Calculate RMS from accumulated values
    iRMS = sqrt(acc / numSamples);
    // Calculate apparent power
    apparentPower = vRMS * iRMS;
    Cayenne.virtualWrite(1, iRMS, "current", "a");
    Cayenne.virtualWrite(2, apparentPower, "pow", "w");
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
