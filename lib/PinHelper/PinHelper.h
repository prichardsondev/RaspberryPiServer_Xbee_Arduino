#pragma once
#include <Arduino.h>

class PinHelper {

    private:
    byte _pinNumber;

    public:
    PinHelper(byte pinNumber);
    void begin();
    void toggle();
    byte pinNumber();
    byte state();
};