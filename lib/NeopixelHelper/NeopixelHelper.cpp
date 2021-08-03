#include "neopixelHelper.h"

NeopixelHelper::NeopixelHelper(uint8_t pin, uint8_t count, long interval)
{
    _pin = pin;
    _count = count;
    _interval = interval;
    _strip = Adafruit_NeoPixel(count, pin, NEO_GRB + NEO_KHZ800);
    _currentPixel = 0;
    _previousMillis = 0;
    callSetColor = false;
}

void NeopixelHelper::begin()
{
    _color = _strip.Color(0, 0, 0);
    _strip.begin();
    _strip.clear();
    _strip.show();
}

void NeopixelHelper::color(uint8_t r, uint8_t g, uint8_t b)
{
    _color = _strip.Color(r, g, b);

    //comment below to turn pixels off with timer... off now!
    if (_color == 0) off();
}

void NeopixelHelper::setColor()
{
    if (callSetColor)
        if ((unsigned long)(millis() - _previousMillis) >= _interval)
        {
            _previousMillis = millis();
            setPixel();
        }
}

void NeopixelHelper::setPixel()
{
    _strip.setPixelColor(_currentPixel, _color);
    _strip.show();
    _currentPixel++;

    if (_currentPixel == _count)
    {
        _currentPixel = 0;
        callSetColor = false;
    }
}

void NeopixelHelper::off()
{
    callSetColor = false;

    for (int i = 0; i < _count; i++)
        _strip.setPixelColor(i, _strip.Color(0, 0, 0));

    _strip.show();
}

int NeopixelHelper::pinNumber(){
    return _pin;
}

uint32_t NeopixelHelper::getColor(){
    return _color;
}
