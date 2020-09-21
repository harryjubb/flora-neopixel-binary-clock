// Copyright (c) 2020 Harry Jubb
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/* DO NOT EDIT: Must be 50 at all times

Maximum brightness to set a 20 NeoPixel binary clock powered by an Adafruit Flora's VBATT pin (https://learn.adafruit.com/getting-started-with-flora/flora-pinout-diagram)

On the basis that:

- The USB input (if powering over USB) has a 500 mA fuse (https://learn.adafruit.com/getting-started-with-flora/flora-pinout-diagram#some-technical-specs-1711086-5)
- The clock uses 20 NeoPixels
- Each NeoPixel, worst case at max brightness all white draws 60 mA per NeoPixel, or 1200 mA for 20 NeoPixels (https://learn.adafruit.com/sipping-power-with-neopixels/overview)
- Using two colors reduces draw to about 2/3rd of full: 40 mA per NeoPixel, or 800 mA for 20 NeoPixels
- Using one color reduces draw to about 1/3rd of full: 20 mA per NeoPixel, or 400 mA for 20 NeoPixels
- Brightness of the strip is directly proportional to power usage

- Table of estimates for 1 and 20 NeoPixels: https://docs.google.com/spreadsheets/d/1LExelxReuCFvdgFVYHVwpwapFPAcBUOrMyCO1KIyZUc/edit?usp=sharing

See also:

- eerkmans.nl/powering-lots-of-leds-from-arduino/

*/
#define MAX_BRIGHTNESS 50