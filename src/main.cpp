// Copyright (c) 2020 harry
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <MaxBrightness.h>

// Pin 6 on the Flora is a PWM pin suitable for the NeoPixel data line
#define STRIP_PIN 6

/*
Always expecting a strip of 20 NeoPixels for a binary-coded decimal clock
Using a different strip length may need a different consideration for MAX_BRIGHTNESS

    8    8    8
    4  4 4  4 4
  2 2  2 2  2 2
  1 1  1 1  1 1

  H H  M M  S S

*/
#define STRIP_LENGTH 20

/*
Do *NOT* set brightness > MAX_BRIGHTNESS (which should be 50 for a "default" 20 NeoPixel setup)
*/
#define BRIGHTNESS 20

// Strip of NeoPixels for the binary clock
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

// Use one color to reduce power usage by 1/3rd
// https://learn.adafruit.com/sipping-power-with-neopixels/insights#strategy-color-selection-2378066-22
uint32_t color = strip.Color(0, 0, 255);

// Setup code here, to run once
void setup()
{
  // Failsafe against setting brightness too high:
  // Never allow setting up NeoPixels with too high a brightness
  if (BRIGHTNESS > MAX_BRIGHTNESS)
  {
    return;
  }

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off' (clearing any previous state)
}

// Main code here, to run repeatedly
void loop()
{
  // Failsafe against setting brightness too high
  if (BRIGHTNESS > MAX_BRIGHTNESS)
  {
    return;
  }
}