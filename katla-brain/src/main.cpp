#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h> 
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <utils.h>

//JST PIN DEFINITIONS
#define PIN1 D10 //DHT pin
#define PIN2 D9
#define PIN3 D8
#define PIN4 D7
#define PIN5 D6
#define PIN6 D5
#define PIN7 D4

//BATTERY VOLTAGE DEFINITIONS
#define PINBAT A0 //battery voltage pin
Battery bat = Battery(PINBAT);

//TEMP MEASURMENT DEFINITIONS
#define PINTHERM1 A1 //thermal sensor 1 pin
#define PINTHERM2 A2 //thermal sensor 2 pin

#define aref_volt 3.3 //measure with multimeter (chose 3.3 because onboard DC converter stability)
Tmp36 tmp36_1 = Tmp36(PINTHERM1, aref_volt);
Tmp36 tmp36_2 = Tmp36(PINTHERM2, aref_volt);

float avgTemp_1;
float avgTemp_2;

//WIFI CONNECTION DEFINITIONS
#define WIFI_SSID "aalto open"
// #define WIFI_SSID "Luolan_Netti"
// #define PSWD ""
String servIP = "http://192.168.1.31:8000/";
String serv_key = "C4Xw4LhlIu";

//TIMING LOOP DEFINITIONS
//Timer for the section that verifies new inputs through GET request
unsigned long timerDelay_GET = 500; //checks new input every half second
unsigned long lastTime_GET = 0;

//Timer for the section that sends sensor data with POST
unsigned long timerDelay_POST = 5000; //sends data to server every timerDelay_POST ms !! should not be less than 2000 for sensors
unsigned long lastTime_POST = 0;

//DHT SENSOR DEFINITIONS
float hum;
#define DHTTYPE DHT11
DHT dht(PIN1, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PINBAT,INPUT); 

  dht.begin();

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID);
  // WiFi.begin(WIFI_SSID, PSWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //setting up the base temperarure reading
  avgTemp_1 = tmp36_1.read_C();
  avgTemp_2 = tmp36_2.read_C();
  for(int i = 0; i < 50; i++){
    avgTemp_1 = avgTemp_1*0.95 + tmp36_1.read_C()*0.05; // each new measurement contributes only 1/10th
    avgTemp_2 = avgTemp_2*0.95 + tmp36_2.read_C()*0.05;
    delay(5);
  }
}

void loop() {

  avgTemp_1 = avgTemp_1*0.95 + tmp36_1.read_C()*0.05;
  avgTemp_2 = avgTemp_2*0.95 + tmp36_2.read_C()*0.05;

  if ((millis() - lastTime_POST) > timerDelay_POST) {
    if ((WiFi.status() == WL_CONNECTED)) {  
    
      //MEASUREMENTS
      float vbat = bat.read_V();
      Serial.print("Battery voltage: ");
      Serial.println(vbat);
      
      //DHT reading and printing
      float hum = dht.readHumidity();
      float temp = dht.readTemperature();

      Serial.print("DHT => Humidity: ");
      Serial.print(hum);
      Serial.print(" Temp: ");
      Serial.println(temp);

      //TMP36 printing values
      Serial.print("TMP36 => Temp1: ");
      Serial.print(avgTemp_1);
      Serial.print(" Temp2: ");
      Serial.println(avgTemp_2);

      //TMP36 printing values voltages
      Serial.print("TMP36 => V1: ");
      Serial.print(analogReadMilliVolts(PINTHERM1));
      Serial.print(" V2: ");
      Serial.println(analogReadMilliVolts(PINTHERM2));

      //SERVER POST
      HTTPClient http;
      http.begin(servIP);
      http.addHeader("Content-Type", "application/json");

      String data_json;
      data_json = "{\"key\": \"" + serv_key + "\",\"sensor_data\": {\"DHT_hum\": " + hum + ", \"DHT_temp\": " + temp + ", \"TMP36_1\": " + avgTemp_1 + ", \"TMP36_2\": " + avgTemp_2 + "}}";
      Serial.println(data_json);
      int httpResponseCode = http.POST(data_json);
      http.end();
    }
    lastTime_POST = millis();
  }
}
