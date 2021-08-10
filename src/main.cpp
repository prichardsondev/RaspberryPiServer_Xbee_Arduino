
#include <Arduino.h>
#include <XBee.h>
#include <SoftwareSerial.h>

#include "xbeeHelper.h"
#include "NeopixelHelper.h"
#include "PinHelper.h"

uint8_t ssRX = 2;
uint8_t ssTX = 3;
SoftwareSerial xbeeSerial(ssRX, ssTX);

uint8_t r, g, b, pin;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

NeopixelHelper strip(8, 7, 500);
NeopixelHelper ring(9, 8, 500);
NeopixelHelper neoPixels[] = {strip, ring};
uint8_t neoPixelLength = *(&neoPixels + 1) - neoPixels;

PinHelper testPin4(4);
PinHelper testPin5(5);
PinHelper testPin7(6);
PinHelper pins[] = {testPin4, testPin5, testPin7};
uint8_t pinsLength = *(&pins + 1) - pins;

/*  
    DEBUG true
    - xbee.begin(xbeeSerial)
    - passes software serial (xbeeSerial) to xbee write calls
    - must have uno rx tx pins 2,3 to xbee tx rx pins 2,3
    DEBUG false
    - xbee.begin(Serial)
    - passes hardward serial (Serial) to xbee write calls
    - must have rx tx pins 0,1 to xbee tx rx pins 2,3

    Note: When uploading code you cannot have hardware tx
    pin connected on uno - disconnect pin 0. Does not matter
    if you are in debug mode using pin 2 for tx

*/
#define DEBUG true

void setup()
{

  for (int i = 0; i < neoPixelLength; i++)
    neoPixels[i].begin();

  for (int i = 0; i < pinsLength; i++)
    pins[i].begin();

  if (DEBUG)
  {
    Serial.begin(9600);
    xbeeSerial.begin(9600);
    xbee.begin(xbeeSerial);
  }
  else
  {
    Serial.begin(9600);
    xbee.begin(Serial);
  }
}

void loop()
{

  xbee.readPacket();
  if (xbee.getResponse().isAvailable())
  {

    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {

      xbee.getResponse().getZBRxResponse(rx);

      if (DEBUG)
      {
        for (int i = 0; i < rx.getDataLength(); i++)
        {
          Serial.print("ascii ");
          Serial.println((char)rx.getData(i));

          Serial.print("dec ");
          Serial.println(rx.getData(i));
        }
      }

      //Update requested
      if (rx.getDataLength() == 6)
      {
        if (DEBUG)
          Serial.println("update");

        for (int i = 0; i < pinsLength; i++)
        {
          byte data[] = {pins[i].state(), pins[i].pinNumber()};

          if (DEBUG)
            writePinState(data, xbeeSerial);
          else
            writePinState(data, Serial);
        }

        for (int i = 0; i < neoPixelLength; i++)
        {
          uint32_t color = neoPixels[i].getColor();
          uint8_t r = color >> 16;
          uint8_t g = color >> 8;
          uint8_t b = color;

          uint8_t data[] = {r, g, b, (uint8_t)neoPixels[i].pinNumber()};

          if (DEBUG)
            writeHex(data, xbeeSerial);
          else
            writeHex(data, Serial);
        }
      }
      //hex color with pin to set
      else if (rx.getDataLength() == 4)
      {
        r = rx.getData(0);
        g = rx.getData(1);
        b = rx.getData(2);
        pin = rx.getData(3);

        if (DEBUG)
        {
          Serial.print("getData[0]: ");
          Serial.println(r);

          Serial.print("getData[1]: ");
          Serial.println(g);

          Serial.print("getData[2]: ");
          Serial.println(b);

          Serial.print("getData[3]: ");
          Serial.println(pin);
        }

        for (int i = 0; i < neoPixelLength; i++)
        {
          if (neoPixels[i].pinNumber() == pin)
          {
            if (DEBUG)
              Serial.println(pin);
            neoPixels[i].color(r, g, b);
            neoPixels[i].callSetColor = true;
            if (DEBUG)
              Serial.println(neoPixels[i].callSetColor);

            //write color to server (coordinator xbee)
            byte data[] = {r, g, b, pin};
            if (DEBUG)
              writeHex(data, xbeeSerial);
            else
              writeHex(data, Serial);

            break;
          }
        }
      }
      //toggle single pin
      else if (rx.getDataLength() == 1)
      {
        // int pin = rx.getData(0) - 48;
        int pin = ((char)rx.getData(0)) - '0';
        for (int i = 0; i < pinsLength; i++)
        {
          if (pins[i].pinNumber() == pin)
          {
            if (DEBUG)
              Serial.println(pin);

            pins[i].toggle();
            byte data[] = {pins[i].state(), pins[i].pinNumber()};

            if (DEBUG)
              writePinState(data, xbeeSerial);
            else
              writePinState(data, Serial);

            break;
          }
        }
      }
    }
  }

  for (int i = 0; i < neoPixelLength; i++)
    neoPixels[i].setColor();
}
