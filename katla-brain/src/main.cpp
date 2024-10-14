#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h> 
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <utils.h>
#include <secrets.h>

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
bool home = 0;
#define OPEN_SSID "aalto open"

String serv = "https://katla.cloud/api/data";

const char *secret = CONTROLLER_SECRET;
WiFiClientSecure client;

const char *cert = "-----BEGIN CERTIFICATE-----\n\
MIIFBTCCAu2gAwIBAgIQS6hSk/eaL6JzBkuoBI110DANBgkqhkiG9w0BAQsFADBP\n\
MQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFy\n\
Y2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBYMTAeFw0yNDAzMTMwMDAwMDBa\n\
Fw0yNzAzMTIyMzU5NTlaMDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBF\n\
bmNyeXB0MQwwCgYDVQQDEwNSMTAwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n\
AoIBAQDPV+XmxFQS7bRH/sknWHZGUCiMHT6I3wWd1bUYKb3dtVq/+vbOo76vACFL\n\
YlpaPAEvxVgD9on/jhFD68G14BQHlo9vH9fnuoE5CXVlt8KvGFs3Jijno/QHK20a\n\
/6tYvJWuQP/py1fEtVt/eA0YYbwX51TGu0mRzW4Y0YCF7qZlNrx06rxQTOr8IfM4\n\
FpOUurDTazgGzRYSespSdcitdrLCnF2YRVxvYXvGLe48E1KGAdlX5jgc3421H5KR\n\
mudKHMxFqHJV8LDmowfs/acbZp4/SItxhHFYyTr6717yW0QrPHTnj7JHwQdqzZq3\n\
DZb3EoEmUVQK7GH29/Xi8orIlQ2NAgMBAAGjgfgwgfUwDgYDVR0PAQH/BAQDAgGG\n\
MB0GA1UdJQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATASBgNVHRMBAf8ECDAGAQH/\n\
AgEAMB0GA1UdDgQWBBS7vMNHpeS8qcbDpHIMEI2iNeHI6DAfBgNVHSMEGDAWgBR5\n\
tFnme7bl5AFzgAiIyBpY9umbbjAyBggrBgEFBQcBAQQmMCQwIgYIKwYBBQUHMAKG\n\
Fmh0dHA6Ly94MS5pLmxlbmNyLm9yZy8wEwYDVR0gBAwwCjAIBgZngQwBAgEwJwYD\n\
VR0fBCAwHjAcoBqgGIYWaHR0cDovL3gxLmMubGVuY3Iub3JnLzANBgkqhkiG9w0B\n\
AQsFAAOCAgEAkrHnQTfreZ2B5s3iJeE6IOmQRJWjgVzPw139vaBw1bGWKCIL0vIo\n\
zwzn1OZDjCQiHcFCktEJr59L9MhwTyAWsVrdAfYf+B9haxQnsHKNY67u4s5Lzzfd\n\
u6PUzeetUK29v+PsPmI2cJkxp+iN3epi4hKu9ZzUPSwMqtCceb7qPVxEbpYxY1p9\n\
1n5PJKBLBX9eb9LU6l8zSxPWV7bK3lG4XaMJgnT9x3ies7msFtpKK5bDtotij/l0\n\
GaKeA97pb5uwD9KgWvaFXMIEt8jVTjLEvwRdvCn294GPDF08U8lAkIv7tghluaQh\n\
1QnlE4SEN4LOECj8dsIGJXpGUk3aU3KkJz9icKy+aUgA+2cP21uh6NcDIS3XyfaZ\n\
QjmDQ993ChII8SXWupQZVBiIpcWO4RqZk3lr7Bz5MUCwzDIA359e57SSq5CCkY0N\n\
4B6Vulk7LktfwrdGNVI5BsC9qqxSwSKgRJeZ9wygIaehbHFHFhcBaMDKpiZlBHyz\n\
rsnnlFXCb5s8HKn5LsUgGvB24L7sGNZP2CX7dhHov+YhD+jozLW2p9W4959Bz2Ei\n\
RmqDtmiXLnzqTpXbI+suyCsohKRg6Un0RC47+cpiVwHiXZAW+cn8eiNIjqbVgXLx\n\
KPpdzvvtTnOPlC7SQZSYmdunr3Bf9b77AiC/ZidstK36dRILKz7OA54=\n\
-----END CERTIFICATE-----";

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

  WiFi.mode(WIFI_STA);
  delay(500);

  Serial.print("Attempting to connect to WPA SSID: ");
  // attempt to connect to Wifi network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    // Connect to WPA/WPA2 network:
    if(home){
      Serial.println(SSID_SECRET);
      WiFi.begin(SSID_SECRET, PSWD_SECRET);
    }else{
      Serial.println(OPEN_SSID);
      WiFi.begin(OPEN_SSID);
    }

    delay(1000);
  }
  Serial.println("\nConnected!");

  client.setCACert(cert);

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
      HTTPClient https;
      if (https.begin(client, serv)) {

        String contentType = "application/json; charset=utf-8";
        String postData = "{ \"secret\": \"" + String(secret) + "\"," +
                          "\"temp_in\":" + String(avgTemp_1) + "," +
                          "\"temp_out\":" + String(temp) + "," +
                          "\"humidity\":" + String(hum) + " }";

        https.addHeader("Content-Type", contentType);
        int statusCode = https.POST(postData);
        String response = https.getString();

        Serial.print("Status code: ");
        Serial.println(statusCode);
        Serial.print("Response: ");
        Serial.println(response);

        https.end();
      } else {
        Serial.println("[HTTPS] Connecting failed!");
      }
    }
    lastTime_POST = millis();
  }
}
