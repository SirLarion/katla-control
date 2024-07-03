#include <Arduino.h>

#include <RH_ASK.h>
#include <SPI.h>

#include <DHT.h>

#include <utils.h>

#define aref_volt 3.3 //measure with multimeter (chose 3.3 because onboard DC converter stability)
int temp1_pin = A0;
Tmp36 tmp36_1 = Tmp36(temp1_pin, aref_volt);
int temp2_pin = A1;
Tmp36 tmp36_2 = Tmp36(temp2_pin, aref_volt);

int DHT_pin = 9;
#define DHTTYPE DHT11
DHT dht(DHT_pin, DHTTYPE);

RH_ASK rf_driver;

unsigned int data_time = 2000;
unsigned long data_timer = 0;

float avgTemp_1;
float avgTemp_2;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);

  dht.begin();
  rf_driver.init();

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

  if ((millis()-data_timer) > data_time){
    char temp1_c[10];
    char temp2_c[10];
    char hum_c[10];
    char tempout_c[10];

    
    // dtostrf(avgTemp_1,8,3,temp1_c);
    // dtostrf(avgTemp_2,8,3,temp2_c);
    // dtostrf(dht.readHumidity(),8,3,hum_c);
    // dtostrf(dht.readTemperature(),8,3,hum_c);


    dtostrf(123.123,8,3,temp1_c);
    dtostrf(123.123,8,3,temp2_c);
    dtostrf(123.123,8,3,hum_c);
    dtostrf(123.123,8,3,tempout_c);

    // Serial.println(atof(temp1_c)); //How to convert string back to float

    char buffer[40];
    sprintf(buffer, "<%s,%s,%s,%s>", temp1_c, temp2_c, hum_c, tempout_c);
    Serial.println(buffer);
    const char *msg = buffer;
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    data_timer = millis();
	}
}