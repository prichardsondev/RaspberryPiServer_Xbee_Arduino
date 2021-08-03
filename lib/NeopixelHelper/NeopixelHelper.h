#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class NeopixelHelper
{
private:
    Adafruit_NeoPixel _strip;
    uint8_t _pin;
    uint8_t _count;
    uint32_t _color;
    

    uint16_t _currentPixel;
    unsigned long _interval;
    unsigned long _previousMillis;

    void setPixel();

public:
    bool callSetColor;

    NeopixelHelper(uint8_t pin, uint8_t count, long interval=50);

    void begin();
    void color(uint8_t r, uint8_t g, uint8_t b);
    void setColor();
    void off();
    int pinNumber();
    uint32_t getColor();
};