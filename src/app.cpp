// #include <Arduino.h>
#include <AfricasTalkingCloud.h>
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "Techies@WorkAgain";
char pass[] = "ccad32da7b5d8";
int  status = WL_IDLE_STATUS;
char message[50];
long value = 0, lastMessage = 0;

WiFiClient atwinc1500;
boolean sandbox = true;
void sandboxCallback(char topic [], byte* payload, unsigned int length);

AfricasTalkingCloudClient client(sandbox, sandboxCallback, atwinc1500);
void _keepAlive();
void connectToWPA();
void printWifiData();
void printCurrentNet();

void setup()
{
    WiFi.setPins(9, 3, 4);
    while(!Serial);
    Serial.begin(9600);
    connectToWPA();
}

void loop()
{
	if (!client.connected())
	{
		_keepAlive();
	}
	if (!client.loop()) {
		Serial.print("Client disconnected...");
		if (client.connect("ATWIC1500", "PlatformIO", "pass")) {
			Serial.println("reconnected.");
		}
		else {
			Serial.println("failed.");
		}
	}
	long now = millis();
	if (now - lastMessage > 5000) {
		lastMessage = now;
		value+=5;
		Serial.println("Something else");
		snprintf(message, 75, "Data #%ld", value);
		Serial.print("Publish message: ");
		Serial.println(message);
		client.publish("PlatformIO/ATWIC1500/readings", message);
	}
}

void sandboxCallback(char topic [], byte* payload, unsigned int length)
{
    Serial.println("Message arrived [");
    Serial.print(topic);
    Serial.print("]");
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.println((char)payload[i]);
    }
    Serial.println();
}
//Reconnect always
void _keepAlive() {

	while (!client.connected()) {
		// Connect to WPA...
		connectToWPA();
		Serial.print(F("Contacting Server: "));
		Serial.println();
		if (client.connect("ATWIC1500", "PlatformIO", "Password")) {
			Serial.println("connected");
			client.publish("PlatformIO/ATWIC1500/birth", "ATWIC1500");
			client.subscribe("PlatformIO/ATWIC1500/commands");
		}
		else {
			Serial.print("Failed, rc=");
			Serial.print(client.state());
			Serial.println(" Retrying...");
			delay(6000);
		}
	}
}

void connectToWPA()
{
	Serial.println(F("Setting up connection...."));
	// check for the presence of the shield:
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("WiFi shield not present");
		// don't continue:
		while (true);
	}
	// attempt to connect to Wifi network:
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to WPA SSID: ");
		Serial.println(ssid);
		// Connect to WPA/WPA2 network:
		status = WiFi.begin(ssid, pass);

		// wait 10 seconds for connection:
		delay(10000);
	}
	// you're connected now, so print out the data:
	Serial.print("You're connected to the network");
	printCurrentNet();
	printWifiData();
}


void printWifiData() {
	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
	Serial.println(ip);

	// print your MAC address:
	byte mac[6];
	WiFi.macAddress(mac);
	Serial.print("MAC address: ");
	Serial.print(mac[5], HEX);
	Serial.print(":");
	Serial.print(mac[4], HEX);
	Serial.print(":");
	Serial.print(mac[3], HEX);
	Serial.print(":");
	Serial.print(mac[2], HEX);
	Serial.print(":");
	Serial.print(mac[1], HEX);
	Serial.print(":");
	Serial.println(mac[0], HEX);

}

void printCurrentNet() {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print the MAC address of the router you're attached to:
	byte bssid[6];
	WiFi.BSSID(bssid);
	Serial.print("BSSID: ");
	Serial.print(bssid[5], HEX);
	Serial.print(":");
	Serial.print(bssid[4], HEX);
	Serial.print(":");
	Serial.print(bssid[3], HEX);
	Serial.print(":");
	Serial.print(bssid[2], HEX);
	Serial.print(":");
	Serial.print(bssid[1], HEX);
	Serial.print(":");
	Serial.println(bssid[0], HEX);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.println(rssi);

	// print the encryption type:
	byte encryption = WiFi.encryptionType();
	Serial.print("Encryption Type:");
	Serial.println(encryption, HEX);
	Serial.println();
}