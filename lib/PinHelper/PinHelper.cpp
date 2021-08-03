#include "PinHelper.h"
#include <Arduino.h>

PinHelper::PinHelper(byte pinNumber){
    _pinNumber = pinNumber;
}

void PinHelper::begin(){
    pinMode(_pinNumber,OUTPUT);
}

void PinHelper::toggle(){
    digitalWrite(_pinNumber, !state());
}

byte PinHelper::state(){
    return digitalRead(_pinNumber);
}

byte PinHelper::pinNumber(){
    return _pinNumber;
}