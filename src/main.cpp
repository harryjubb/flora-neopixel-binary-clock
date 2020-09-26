// Copyright (c) 2020 Harry Jubb
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Assuming the Adafruit version of the DS3231 Precision Real Time Clock (RTC)
// then the Adafruit fork of the library must be used
// http://github.com/adafruit/RTClib
#include <RTClib.h>

#include <MaxBrightness.h>

// Pin 6 on the Flora is a PWM pin suitable for the NeoPixel data line
#define STRIP_PIN 6

/*
Always expecting a strip of 20 NeoPixels for a binary-coded decimal clock
Using a different strip length may need a different consideration for MAX_BRIGHTNESS

As diagrammed:

    8    8    8                                                          14      7      0
    4  4 4  4 4                                                          15  13  8   6  1
  2 2  2 2  2 2      Equivalent to NeoPixel strip array indices:      19 16  12  9   5  2
  1 1  1 1  1 1                                                       18 17  11 10   4  3

  H H  M M  S S                                                        H  H   M  M   S  S

*/
#define STRIP_LENGTH 20

/*
Do *NOT* set brightness > MAX_BRIGHTNESS (which should be 50 for a "default" 20 NeoPixel setup)
*/
#define BRIGHTNESS 40

// Strip of NeoPixels for the binary clock
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

// Use one color to reduce power usage by 1/3rd
// https://learn.adafruit.com/sipping-power-with-neopixels/insights#strategy-color-selection-2378066-22
uint32_t color = strip.Color(0, 0, 255);

// Initialise Adafruit DS3231 Real-time clock
RTC_DS3231 rtc;

// Setup code here, to run once
void setup()
{
  strip.begin();
  strip.setBrightness(min(BRIGHTNESS, MAX_BRIGHTNESS));
  strip.show(); // Initialize all pixels to 'off' (clearing any previous state)

  Serial.begin(57600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

// Main code here, to run repeatedly
void loop()
{
  // Get the current time from the RTC
  DateTime now = rtc.now();

  // Convert seconds to string, adding a leading zero if needed

  // Convert minutes to string, adding a leading zero if needed

  // Convert hours to string, adding a leading zero if needed

  // Concatenate HHMMSS into a single time number

  // Use the time number as a bitmask to check whether a NeoPixel should be on

  // TODO: Toggle switch for daylight savings time?

  // TODO: Set the NeoPixel "bits" according to the current time
  for (int i = 0; i < 20; i++) {
    // strip.setPixelColor()
  }

  // Update the NeoPixel strip
  strip.show();

  // Wait for the next second to elapse
  delay(1000);
}