// Including Arduino.h is required for using Serial functions
#include "Arduino.h"

/* Necessary Includes */
#include <ESP8266WiFi.h> // we need wifi to get internet access
#include <time.h> // for time() ctime()

#ifndef STASSID
#	define STASSID "potatopotato" // set your SSID
#	define STAPSK "spudspud" // set your wifi password
#endif

/* Configuration of NTP */
#define MY_NTP_SERVER "at.pool.ntp.org"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"

/* Globals */
time_t now; // this are the seconds since Epoch (1970) - UTC
tm tm; // the structure tm holds time information in a more convenient way

void showTime() {
	time(&now); // read the current time
	localtime_r(&now, &tm); // update the structure tm with the current time
	Serial.print(tm.tm_year + 1900); // years since 1900
	Serial.print("//");
	Serial.print(tm.tm_mon + 1); // January = 0 (!)
	Serial.print("//");
	Serial.print(tm.tm_mday); // day of month
	Serial.print(":");
	Serial.print(tm.tm_hour); // hours since midnight  0-23
	Serial.print("\t:");
	Serial.print(tm.tm_min); // minutes after the hour  0-59
	Serial.print(":");
	Serial.print(tm.tm_sec); // seconds after the minute  0-61*

	Serial.println();
}

// the setup routine runs once when you press reset:
int LED_USE = 13;

void setup() {
	// initialize serial communication at 9600 bits per second:
	Serial.begin(9600);
	Serial.begin(115200);
	Serial.println("\nNTP TZ DST - bare minimum");

	// print out hello world
	Serial.println("Hello World");

	// Setup to blink the inbuilt LED
#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
#endif

	// blink additional led
	pinMode(LED_USE, OUTPUT);


	configTime(MY_TZ, MY_NTP_SERVER); // --> Here is the IMPORTANT ONE LINER needed in your sketch!

	// start network
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.begin(STASSID, STAPSK);
	while(WiFi.status() != WL_CONNECTED) {
		delay(200);
		Serial.print(".");
	}
	Serial.println("\nWiFi connected");
	// by default, the NTP will be started after 60 secs
}


// the loop routine runs over and over again forever:
void loop() {
	// display time
	showTime();

	// Blink the inbuilt LED
#ifdef LED_BUILTIN
	digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
	delay(1000); // wait for a second
	digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
	delay(1000); // wait for a second
#endif

	// blink additional led
	digitalWrite(LED_USE, HIGH); // turn the LED on (HIGH is the voltage level)
	delay(1000); // wait for a second
	digitalWrite(LED_USE, LOW); // turn the LED off by making the voltage LOW
	delay(1000); // wait for a second
}
