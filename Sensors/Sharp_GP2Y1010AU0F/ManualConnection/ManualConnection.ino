// This example shows how to connect Sharp GP2Y1010AU0F to Cayenne using a manually specified Ethernet connection and send/receive sample data.

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTEthernet.h>
//#include <CayenneMQTTEthernetW5500.h> // Uncomment this and comment out CayenneEthernet.h to use an Ethernet 2 shield or other Ethernet W5500 shield.
                                        // You will need the Ethernet2 library installed. See the ArduinoEthernetW5500 example sketch for more info.
//#include <CayenneMQTTEthernetW5200.h> // Uncomment this and comment out CayenneEthernet.h to use an Ethernet W5200 shield.
                                        // You will need the EthernetW5200 library installed. See the ArduinoEthernetW5200 example sketch for more info.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip(10, 0, 0, 20);
IPAddress dns_ip(8, 8, 8, 8);
IPAddress gateway_ip(10, 0, 0, 1);
IPAddress subnet_mask(255, 255, 255, 0);

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
	Cayenne.begin(username, password, clientID, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
  pinMode(ledPin,OUTPUT);
}

void loop() {
	Cayenne.loop();
	//Publish data every 1 seconds (1000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		Cayenne.virtualWrite(0, lastMillis);

    digitalWrite(ledPin,LOW);
    delayMicroseconds(timeMeasuring);
    voltageAnalog = analogRead(measuringPin);
    delayMicroseconds(timeStabilization);
    digitalWrite(ledPin,HIGH);
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
