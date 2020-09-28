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

// Declare functions
String leftPadZero(String, int);

// Map of NeoPixel indices (list indices) to the bit they represent
// in a concatenated bitstring representation of the clock positions
// Written out in full for clarity
unsigned long pixelMap[] = {
  // Seconds small
  0b00000000000000001000, // == 0x8 == 8
  0b00000000000000000100, // == 0x4 == 4
  0b00000000000000000010, // == 0x2 == 2
  0b00000000000000000001, // == 0x1 == 1
  
  // Seconds large
  0b00000000000000010000, // == 0x10 == 16
  0b00000000000000100000, // == 0x20 == 32
  0b00000000000001000000, // == 0x40 == 64
  
  // Minutes small
  0b00000000010000000000, // == 0x400 == 1024
  0b00000000001000000000, // == 0x200 == 512
  0b00000000000100000000, // == 0x100 == 256
  0b00000000000010000000, // == 0x80 == 128
  
  // Minutes large
  0b00000000100000000000, // == 0x800 == 2048
  0b00000001000000000000, // == 0x1000 == 4096
  0b00000010000000000000, // == 0x2000 == 8192
  
  // Hours small
  0b00100000000000000000, // == 0x20000 == 131072
  0b00010000000000000000, // == 0x10000 == 65536
  0b00001000000000000000, // == 0x8000 == 32768
  0b00000100000000000000, // == 0x4000 == 16384
  
  // Hours large
  0b01000000000000000000, // == 0x40000 == 262144
  0b10000000000000000000, // == 0x80000 == 524288
};

// Strip of NeoPixels for the binary clock
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

// Use one color to reduce power usage by 1/3rd
// https://learn.adafruit.com/sipping-power-with-neopixels/insights#strategy-color-selection-2378066-22
uint32_t offColor = strip.Color(0, 0, 0);
uint32_t seconds1Color = strip.gamma32(strip.ColorHSV(40960));
uint32_t seconds2Color = strip.gamma32(strip.ColorHSV(32768));
uint32_t minutes1Color = strip.gamma32(strip.ColorHSV(24576 ));
uint32_t minutes2Color = strip.gamma32(strip.ColorHSV(16384));
uint32_t hours1Color = strip.gamma32(strip.ColorHSV(8192));
uint32_t hours2Color = strip.gamma32(strip.ColorHSV(0));

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

  Serial.println("Setting the time anyway");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

// Main code here, to run repeatedly
int32_t currentColor;
void loop()
{
  // Get the current time from the RTC
  DateTime now = rtc.now();

  // TODO: Toggle switch for daylight savings time?

  // Convert hours to string
  String hours = leftPadZero(String(now.hour()), 2);
  Serial.println(hours);

  // Convert minutes to string
  String minutes = leftPadZero(String(now.minute()), 2);
  Serial.println(minutes);

  // Convert seconds to string
  String seconds = leftPadZero(String(now.second()), 2);
  Serial.println(seconds);

  // Split apart individual digits
  char hours1 = hours[0];
  char hours2 = hours[1];
  char minutes1 = minutes[0];
  char minutes2 = minutes[1];
  char seconds1 = seconds[0];
  char seconds2 = seconds[1];

  Serial.println();
  Serial.println(hours1);
  Serial.println(hours2);
  Serial.println(minutes1);
  Serial.println(minutes2);
  Serial.println(seconds1);
  Serial.println(seconds2);

  // Convert the individual digits to bitstrings, padded with zeros
  // to reach the length of each digit's binary representation on the clock
  String hours1Bitstring = leftPadZero(String(String(hours1).toInt(), BIN), 2);
  String hours2Bitstring = leftPadZero(String(String(hours2).toInt(), BIN), 4);
  String minutes1Bitstring = leftPadZero(String(String(minutes1).toInt(), BIN), 3);
  String minutes2Bitstring = leftPadZero(String(String(minutes2).toInt(), BIN), 4);
  String seconds1Bitstring = leftPadZero(String(String(seconds1).toInt(), BIN), 3);
  String seconds2Bitstring = leftPadZero(String(String(seconds2).toInt(), BIN), 4);

  Serial.println();
  Serial.println(hours1Bitstring);
  Serial.println(hours2Bitstring);
  Serial.println(minutes1Bitstring);
  Serial.println(minutes2Bitstring);
  Serial.println(seconds1Bitstring);
  Serial.println(seconds2Bitstring);

  // Concatenate HHMMSS into a single time bitstring
  String hhmmssBitstring = String(
    hours1Bitstring +
    hours2Bitstring +
    minutes1Bitstring +
    minutes2Bitstring +
    seconds1Bitstring +
    seconds2Bitstring
  );

  Serial.println();
  Serial.println(hhmmssBitstring);

  // Use the HHMMSS clock-formatted bitstring
  // as a bitmask to check whether a NeoPixel should be on
  unsigned long hhmmss = strtoul(hhmmssBitstring.c_str(), NULL, BIN);
  Serial.println(hhmmss);

  Serial.println();
  
  // Set the NeoPixel "bits" according to the current time
  for (int i = 0; i < 20; i++) {
    Serial.print("NeoPixel ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(hhmmss & pixelMap[i] ? "ON" : "OFF");

    if (hhmmss & pixelMap[i]) {
      if (i <= 3) {
        currentColor = seconds1Color;
      } else if (i <= 6) {
        currentColor = seconds2Color;
      } else if (i <= 10) {
        currentColor = minutes1Color;
      } else if (i <= 13) {
        currentColor = minutes2Color;
      } else if (i <= 17) {
        currentColor = hours1Color;
      } else {
        currentColor = hours2Color;
      }
    } else {
      currentColor = offColor;
    }

    strip.setPixelColor(i, currentColor);
  }

  // Update the NeoPixel strip
  strip.show();

  // Wait for the next second to elapse
  Serial.println();
  delay(1000);
}

// Leftpad a string with zero until the string is the desired length
String leftPadZero(String theString, int desiredLength) {
  String stringCopy = String(theString);

  while (stringCopy.length() < desiredLength) {
    stringCopy = String('0') + stringCopy;
  }

  return stringCopy;
}