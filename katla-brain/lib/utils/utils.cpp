#include <Arduino.h>
#include <utils.h>

Tmp36::Tmp36(int pin, float vref){
  _pin = pin;
  _vref = vref;
  pinMode(_pin,INPUT);
}

float Tmp36::read_C(){
  float v = analogRead(_pin)*_vref;
  v/=4095.0;
  return (v-0.5)*100;
}

float Tmp36::read_F(){
  float v = analogRead(_pin)*_vref;
  v/=1024.0;
  return (((v-0.5)*100)*9.0/5)+32.0;
}

Battery::Battery(int pin){
  _pin = pin;
  pinMode(_pin, INPUT);
}

float Battery::read_V(){
  float vbat = 0;
  for(int i=0; i<16; i++){
    vbat += analogReadMilliVolts(_pin);
  }
  return 2*vbat/16/1000;
}