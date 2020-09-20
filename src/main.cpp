#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define STRIP_PIN 6
#define STRIP_LENGTH 20
#define BRIGHTNESS 20

// Strip of NeoPixels for the binary clock
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Setup code here, to run once
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off' (clearing any previous state)
}

void loop() {
  // Main code here, to run repeatedly
}