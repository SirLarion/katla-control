#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

const int PIN1 = D10; //DHT pin
const int PIN2 = D9;
const int PIN3 = D8;
const int PIN4 = D7;

#define WIFI_SSID "aalto open"

int refr = 2000;
unsigned long prev_refr = 0;  // last time messages' scan has been done

float temp_in_1, temp_in_2, temp_out;
float hum;

// Specify the type of DHT sensor being used.
#define DHTTYPE DHT11
// Initialize the sensor with the pin and type.
DHT dht(PIN1, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht.begin();

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID);
  // WiFi.begin(WIFI_SSID, PSWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

void loop() {
  if (millis() - prev_refr > refr) {
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    // Measuring Wifi strength
    // Serial.print(WiFi.SSID());
    // Serial.print("; RSSI: ");
    // Serial.println(WiFi.RSSI());

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" Temp: ");
    Serial.println(temp);

    prev_refr = millis();
  }
}
