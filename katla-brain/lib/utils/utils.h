#include <Arduino.h>

class Tmp36{
  public:
    Tmp36(int pin, float vref);
    float read_C();
    float read_F();

  private:
    int _pin;
    float _vref;
};

class Battery{
  public:
    Battery(int pin);
    float read_V();
  
  private:
    int _pin;
};