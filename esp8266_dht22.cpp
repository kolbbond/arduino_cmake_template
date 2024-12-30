#include <ESP8266WiFi.h>

// define esp8266 pin conversion?

// DHT
#include <DHT.h>
#define DHT_SENSOR_PIN 2 // The ESP8266 pin D4 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT22

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// network connection
const char* ssid = "potatopotato";
const char* password = "spudspud";

int ledPin = 13; // GPIO13

// create a server on port 80
WiFiServer server(80);

bool connect_to_wifi() {
	// Connect to WiFi network
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while(WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");

	// Start the server
	server.begin();
	Serial.println("Server started");

	// Print the IP address
	Serial.print("Use this URL to connect: ");
	Serial.print("http://");
	Serial.print(WiFi.localIP());
	Serial.println("/");

	// default success?
	return true;
}

void setup() {
	// open serial
	Serial.begin(115200);
	Serial.println();
	delay(10);

	// setup pins
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);

	// wifi connection
	connect_to_wifi();

	// start dht11
	dht_sensor.begin(); // initialize the DHT sensor
}

void loop() {
	// Check if a client has connected
	WiFiClient client = server.available();
	if(!client) {
		return;
	}

	// Wait until the client sends some data
	Serial.println("new client");
	while(!client.available()) {
		delay(1);
	}

	// Read the first line of the request
	String request = client.readStringUntil('\r');
	Serial.println(request);
	client.flush();

	enum { LEDON, LEDOFF, TIME, DATA };

	// Match the requests
	int reqtype;
	int value = LOW;
	int timemillis = millis();
	if(request.indexOf("/LED=ON") != -1) {
		digitalWrite(ledPin, HIGH);
		value = HIGH;
		reqtype = LEDON;
	}
	if(request.indexOf("/LED=OFF") != -1) {
		digitalWrite(ledPin, LOW);
		value = LOW;
		reqtype = LEDOFF;
	}
	if(request.indexOf("/time") != -1) {
		Serial.println("requesting time...");
		Serial.println(timemillis);
		reqtype = TIME;

		// flicker light for every request
		digitalWrite(ledPin, HIGH);
		delay(1);
		digitalWrite(ledPin, LOW);
	}
	if(request.indexOf("/data") != -1) {
		Serial.println("requesting data...");
		reqtype = DATA;

		// flicker light for every request
		digitalWrite(ledPin, HIGH);
		delay(1);
		digitalWrite(ledPin, LOW);
	}

	// Set ledPin according to the request
	//digitalWrite(ledPin, value);

	if(reqtype == LEDON || reqtype == LEDOFF) {
		// Return the response
		client.println("HTTP/1.1 200 OK");
		client.println("Content-Type: text/html");
		client.println(""); //  do not forget this one
		client.println("<!DOCTYPE HTML>");
		client.println("<html>");

		client.print("Led pin is now: ");

		if(value == HIGH) {
			client.print("On");
		} else {
			client.print("Off");
		}
		client.println("<br><br>");
		client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
		client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
		client.println("</html>");
	} else if(reqtype == TIME) {

		client.println("HTTP/1.1 200 OK");
		client.println("Content-Type: text/html");
		client.println(""); //  do not forget this one
		client.print(timemillis); //  do not forget this one

	} else if(reqtype == DATA) {
		// only read if requested?
		float humi = dht_sensor.readHumidity();
		// read temperature in Celsius

		// reading multiple units is slow (just convert between units if needed)
		//float temperature_C = dht_sensor.readTemperature();
		float temperature_C = 0;

		// read temperature in Fahrenheit
		float temperature_F = dht_sensor.readTemperature(true);

		// check whether the reading is successful or not
		if(isnan(temperature_C) || isnan(temperature_F) || isnan(humi)) {
			Serial.println("Failed to read from DHT sensor!");
		} else {
			Serial.print("Humidity: ");
			Serial.print(humi);
			Serial.print("%");

			Serial.print("  |  ");

			Serial.print("Temperature: ");
			//Serial.print(temperature_C);
			Serial.print("°C  ~  ");
			Serial.print(temperature_F);
			Serial.println("°F");

			// print to server
			client.println("HTTP/1.1 200 OK");
			client.println("Content-Type: text/html");
			client.println(""); //  do not forget this one
			client.print("Temperature [°F]: ");
			client.print(temperature_F);
			client.print("  |  Humidity [%]: ");
			client.print(humi);
		}

	} else {
		// delay if no request
		delay(1);
	}

	//delay(1);
	//Serial.println("Client disonnected");
	//Serial.println("");
}
