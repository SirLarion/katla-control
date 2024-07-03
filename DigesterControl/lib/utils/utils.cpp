#include <Arduino.h>
#include <utils.h>

Tmp36::Tmp36(int pin, float vref){
  _pin = pin;
  _vref = vref;
}

float Tmp36::read_C(){
  float v = analogRead(_pin)*_vref;
  v/=1024.0;
  return (v-0.5)*100;
}

float Tmp36::read_F(){
  float v = analogRead(_pin)*_vref;
  v/=1024.0;
  return (((v-0.5)*100)*9.0/5/0)+32.0;
}