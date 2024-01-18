/*
 * File: main.cpp
 * Copyright Tauno Erik
 * Created: 24.12.2023
 * Last edited: 18.01.2024
 * Version:
 * Description: RGB LED circle
 * Hardware:
 *   Pi Pico
 *   1000 uF CAPACITOR between NeoPixel strip's + and - connections
 *   DATA-IN should pass through a 300-500 OHM RESISTOR.
 *   When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
 *   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED
 *   MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
 * 
 * Vaata: https://github.com/FastLED/FastLED
 */

#include <Arduino.h>
#include "Adafruit_GFX.h"
// #include "Adafruit_NeoMatrix.h"
#include "Adafruit_NeoPixel.h"

const uint LED_PIN = 6;

// LEDide arv igas ringis
const uint CIRCLE_1_NUM = 32;
const uint CIRCLE_2_NUM = 32;
const uint CIRCLE_3_NUM = 32;
const uint CIRCLE_4_NUM = 26;
const uint CIRCLE_5_NUM = 24;
const uint CIRCLE_6_NUM = 18;
const uint CIRCLE_7_NUM = 18;
const uint CIRCLE_8_NUM = 14;
const uint CIRCLE_9_NUM = 7;
const uint CIRCLE_10_NUM = 1;

const uint NUM_OF_CIRCLES = 10;

const uint CIRCLES[NUM_OF_CIRCLES + 1] = {0, CIRCLE_1_NUM, CIRCLE_2_NUM,
                                      CIRCLE_3_NUM, CIRCLE_4_NUM,
                                      CIRCLE_5_NUM, CIRCLE_6_NUM,
                                      CIRCLE_7_NUM, CIRCLE_8_NUM,
                                      CIRCLE_9_NUM, CIRCLE_10_NUM
                                     };

// Kokku LEDe
const uint LED_COUNT = CIRCLE_1_NUM + CIRCLE_2_NUM + CIRCLE_3_NUM
                     + CIRCLE_4_NUM + CIRCLE_5_NUM + CIRCLE_6_NUM
                     + CIRCLE_7_NUM + CIRCLE_8_NUM + CIRCLE_9_NUM
                     + CIRCLE_10_NUM;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i=0; i < pixels.numPixels(); i++) {  // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for (int a=0; a < 10; a++) {  // Repeat 10 times...
    for (int b=0; b < 3; b++) {  //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c=b; c < pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color);  // Set pixel 'c' to value 'color'
      }
      pixels.show();  // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for (long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    pixels.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    pixels.show();  // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a=0; a < 30; a++) {  // Repeat 30 times...
    for (int b=0; b < 3; b++) {  //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c=b; c < pixels.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue));  // hue -> RGB
        pixels.setPixelColor(c, color);  // Set pixel 'c' to value 'color'
      }
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90;  // One cycle of color wheel over 90 frames
    }
  }
}

/*
Ükshaaval kõik*/
void one_by_one(int delay_val) {
  pixels.clear();
  for (uint i = 0; i < LED_COUNT; i++) {
    int r = random(255);
    int g = random(255);
    int b = random(255);
     pixels.clear();
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
    delay(delay_val);
  }
}

/*
Väljast sisse
*/
void ringid_out_to_in(int delay_val) {
  uint start = 0;
  uint end = 0;

  for (uint i = 0; i < NUM_OF_CIRCLES; i++) {
    start += CIRCLES[i];
    end += CIRCLES[i+1];
    // Serial.print("i=");
    // Serial.print(i);
    // Serial.print(" start=");
    // Serial.print(start);
    // Serial.print(" end=");
    // Serial.print(end);

    pixels.clear();

    for (int p = start; p < end; p++) {
      pixels.setPixelColor(p, pixels.Color(20, 70, 0));
      // Serial.print(" p=");
      // Serial.print(i);
    }
    // Serial.println();

    pixels.show();
    delay(delay_val);
  }
  // Serial.println();
}

/*
Seest välja
*/
void ringid_in_to_out(int delay_val) {
  uint start = 0;
  uint end = 0;

  for (uint i = 0; i < NUM_OF_CIRCLES; i++) {
    start += CIRCLES[i];
  }

  for (uint i = 1; i <= NUM_OF_CIRCLES+1; i++) {
    end += CIRCLES[i];
  }

  for (uint i = NUM_OF_CIRCLES; i > 0; i--) {
    // Serial.print("i=");
    // Serial.print(i);
    // Serial.print(" start=");
    // Serial.print(start);
    // Serial.print(" end=");
    // Serial.print(end);

    pixels.clear();

    for (int p = start; p < end; p++) {
      pixels.setPixelColor(p, pixels.Color(20, 70, 0));
      // Serial.print(" p=");
      // Serial.print(i);
    }
    // Serial.println();

    pixels.show();
    delay(delay_val);
    start -= CIRCLES[i-1];
    end -= CIRCLES[i];
  }
  // Serial.println();
}

/*
Sees välja koos värviga
*/
void ringid_in_to_out(int delay_val, uint32_t colour) {
  uint start = 0;
  uint end = 0;

  for (uint i = 0; i < NUM_OF_CIRCLES; i++) {
    start += CIRCLES[i];
  }

  for (uint i = 1; i <= NUM_OF_CIRCLES+1; i++) {
    end += CIRCLES[i];
  }

  for (uint i = NUM_OF_CIRCLES; i > 0; i--) {
    // Serial.print("i=");
    // Serial.print(i);
    // Serial.print(" start=");
    // Serial.print(start);
    // Serial.print(" end=");
    // Serial.print(end);

    pixels.clear();

    for (int p = start; p < end; p++) {
      pixels.setPixelColor(p, colour);
      // Serial.print(" p=");
      // Serial.print(i);
    }
    // Serial.println();

    pixels.show();
    delay(delay_val);
    start -= CIRCLES[i-1];
    end -= CIRCLES[i];
  }
  // Serial.println();
}

/*
Tester
One by one RGB
https://github.com/tigoe/NeoPixel_examples/blob/main/NeoPixelTester/NeoPixelTester.ino
*/

void tester(int delay_val) {
  static uint64_t color = 0xFF;  // start with blue

  for (int i = 0; i < LED_COUNT; i++) {
    pixels.setPixelColor(i, color);

    if (i > 0) {
      pixels.setPixelColor(i - 1, 0);  // turn off the last pixel
    }
    Serial.println(color, HEX);
    pixels.show();
    delay(delay_val);
  }
  Serial.println(color, HEX);
  delay(100);

  if (color == 0xFF0000) {  // if the color is white (0xFF000000)
    color = 0xFF;            // then set it back to blue
    // note: if you are using RGB instead of RGBW pixels, use
    // 0xFF0000 for your highest color instead of 0xFF000000
  } else {
    // shift the lit color (value FF, or 255) to the next color
    color = color << 8;
  }

  pixels.clear();
}

/*

*/
void hex_color() {
  static uint32_t color = 0x00FF00;
 /*
    Each byte of the color is a hue, in the sequence WWRRGGBB.
    0xFF000000 is white
    0x00FF0000 is red
    0x0000FF00 is green
    0x000000FF is blue

    Other colors are made by combining, e.g.
    0x000077FF is teal
    0x003300FF is violet
  */
  Serial.println(color, HEX);

  // loop over all the pixels:
  for (int pixel = 0; pixel < LED_COUNT; pixel++) {
    pixels.setPixelColor(pixel, color);
  }
  pixels.show();
  delay(1000);
  if (color >= 0xFF0000) {
    color = 0xFF;
  } else {
    color = color << 8;
  }
}


/*
Üks LED sisse ja välja
*/
void bounce(int delay_val) {
  static int thisPixel = 0;
  static int lastPixel = LED_COUNT-1;
  static int countDirection = 1;

  // increment the current pixel number:
  thisPixel += countDirection;

  // if the current pixel is at either end of the strip,
  // reverse the direction of counting:
  if (thisPixel < 0 || thisPixel >= LED_COUNT) {
    countDirection = -countDirection;
  }
  // set the current and last pixels:
  pixels.setPixelColor(thisPixel, 0x0000FF);
  pixels.setPixelColor(lastPixel, 0x000000);
  lastPixel = thisPixel;

  // update the strip:
  pixels.show();
  // delay 0.1 seconds:
  delay(delay_val);
}

void fade_all(int max_brightness) {
  int r = random(255);
  int g = random(255);
  int b = random(255);

  // loop from brightest to dimmest:
  for (int brightness = max_brightness; brightness >= 0; brightness--) {
    for (int i = 0; i < LED_COUNT; i++) {
      pixels.setBrightness(brightness);
      // brightness just acts as a limit on whatever color you set, so
      // you still have to set color for all the pixels, but you'll get
      // a dimmed version of the color you set:
      // loop over all the pixels, turn on one at a time:
      // pixels.setPixelColor(i, 0x15CC00);
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
    delay(10);
  }
  delay(100);

  for (int brightness = 0; brightness <= max_brightness; brightness++) {
    for (int i = 0; i < LED_COUNT; i++) {
      pixels.setBrightness(brightness);
      // pixels.setPixelColor(i, 0x15CC00);
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
    delay(10);
  }
  delay(100);
}

/*
https://github.com/tigoe/NeoPixel_examples/blob/main/NeoPixelFadeChase/NeoPixelFadeChase.ino
*/
void fade_chase() {
  const int trailLength = LED_COUNT/12;  // length of the fading trail
  static uint32_t trailColor[LED_COUNT];  // array of colors for the trail
  static int currentPixel = 0;                 // the current pixel
  static int trailingPixel = LED_COUNT;        // the final trailing pixel
  static int trailDirection = 1;               // direction of the trail
  static bool init = true;

  // init params
  if (init) {
    init = false;
    for (int p = 0; p < trailLength; p++) {
      trailColor[p] = (p * 255) / 8;
    }
    for (int p = 9; p < 12; p++) {
      trailColor[p] = 255 / (p -8);
    }
    // fill the rest with zeroes:
    for (int p = 12; p < LED_COUNT; p++) {
      trailColor[p] = 0;
    }
  }

  // increment the current pixel number:
  currentPixel += trailDirection;

  // if the current pixel is at either end of the strip,
  // reverse the direction of counting:
  if (currentPixel < 0 || currentPixel >= LED_COUNT) {
    trailDirection = -trailDirection;
  }

  // set the current pixel:
  pixels.setPixelColor(currentPixel, trailColor[currentPixel]);

  // copy the current pixel number for setting the trail:
  trailingPixel = currentPixel;

  // while the trailing pixel number is still in range:
  while (trailingPixel < LED_COUNT && trailingPixel >= 0) {
    // color number is the distance from current pixel in either direction:
    int colorNum = abs(currentPixel - trailingPixel);
    // set the color:
    pixels.setPixelColor(trailingPixel, trailColor[colorNum]);
    // move to the next trailing pixel:
    trailingPixel -= trailDirection;
  }

  // update the strip:
  pixels.show();
  // delay 0.1 seconds:
  delay(100);
}

/*
uint32_t color
*/
void sektor(int delay_val) {
  static uint8_t r = 100;
  static uint8_t g = 0;
  static uint8_t b = 200;
  static int8_t r_direction = -1;
  static int8_t g_direction = 1;
  static int8_t b_direction = 1;

  static int sektor_0[7] = {0, 1, 2, 3, 4, 5, 6};
  static int sektor_1[7] = {32, 33, 34, 35, 36, 37, 38};
  static int sektor_2[7] = {64, 65, 66, 67, 68, 69, 70};
  static int sektor_3[6] = {96, 97, 98, 99, 100, 101};

  // Sektor 0
  for (int i = 0; i < 7; i++) {
    if (i < 6) {
      pixels.setPixelColor(sektor_3[i], pixels.Color(r, g, b));
    }
    pixels.setPixelColor(sektor_2[i], pixels.Color(r, g, b));
    pixels.setPixelColor(sektor_1[i], pixels.Color(r, g, b));
    pixels.setPixelColor(sektor_0[i], pixels.Color(r, g, b));
  }
  pixels.show();
  delay(delay_val);

  for (int i = 0; i < 7; i++) {
    // Update sektor 0  pixels
    if (sektor_0[i] == 31) {
      sektor_0[i] = 0;
    } else {
      sektor_0[i] = sektor_0[i] + 1;
    }
    // Update sektor 1  pixels
    if (sektor_1[i] == 63) {
      sektor_1[i] = 32;  // start pixel
    } else {
      sektor_1[i] = sektor_1[i] + 1;
    }
    // Update sektor 2  pixels
    if (sektor_2[i] == 95) {
      sektor_2[i] = 64;  // start pixel
    } else {
      sektor_2[i] = sektor_2[i] + 1;
    }
    // Update sektor 3  pixels
    if (i < 6) {
      if (sektor_3[i] >= 121) {
        sektor_3[i] = 96;  // start pixel
      } else {
      sektor_3[i] = sektor_3[i] + 1;
      }
    }
  }

  // change color directions
  if (r <= 1 && r_direction < 0) {
    r_direction = -r_direction;
    // Serial.print("r_direction=");
    // Serial.println(r_direction);
  }
  if (r == 254 && r_direction > 0) {
    r_direction = -r_direction;
    // Serial.print("r_direction=");
    // Serial.println(r_direction);
  }

  if (g <= 1 && g_direction < 0) {
    g_direction = -g_direction;
    // Serial.print("g_direction=");
    // Serial.println(g_direction);
  }
  if (g == 254 && g_direction > 0) {
    g_direction = -g_direction;
    // Serial.print("g_direction=");
    // Serial.println(g_direction);
  }

  if (r <= 1 && r_direction < 0) {
    r_direction = -r_direction;
    // Serial.print("r_direction=");
    // Serial.println(r_direction);
  }
  if (b == 254 && b_direction > 0) {
    b_direction = -b_direction;
    // Serial.print("b_direction=");
    // Serial.println(b_direction);
  }

  r += r_direction;
  g += g_direction;
  b += b_direction;

  // Serial.print("g=");
  // Serial.print(g);
  // Serial.print(" r=");
  // Serial.print(r);
  // Serial.println();

  pixels.clear();
}

/*
kaar 1-10
ei tööta hästi
*/
void kaar(int kaar, int lenght, uint32_t color, int delay_val) {
  static uint64_t previous_millis = 0;
  static int circle_start = 0;
  static int circle_end = 0;
  static int sektor[CIRCLE_1_NUM] = {0};  // 32 max ring
  static bool initialize = true;

  // täida sektor esimesel korral
  if (initialize) {
    Serial.print("Initialize");
    initialize = false;

    switch (kaar) {
      case 1:
        circle_start = CIRCLES[0];
        circle_end = CIRCLES[1];
        break;
      case 2:
        circle_start = 32;
        circle_end = 64;
        break;
      default:
        circle_start = 10;
        circle_end = 11;
        break;
    }

    for (int i = 0; i < lenght; i++) {
      sektor[i] = circle_start + i;
    }
  }


  if ((millis()-previous_millis) >= delay_val) {
    previous_millis = millis();

    // pixels.clear();

    for (int i = 0; i < lenght; i++) {
      pixels.setPixelColor(sektor[i], color);
    }

    pixels.show();

    for (int i = 0; i < lenght; i++) {
      if (sektor[i] == circle_end-1) {
        sektor[i] = circle_start;
      } else {
        sektor[i] = sektor[i] + 1;
      }
    }
  }
}

/*
 * --------------------------------------------------
 * --- Celestial Object -----------------------------
 */
void celestial_object() {
  static uint32_t LED_colors[LED_COUNT] = {0};
  static bool initialize = true;
  static bool colors_changed = false;
  static bool update[NUM_OF_CIRCLES] = {false};
  uint delays[NUM_OF_CIRCLES] = {65, 55, 75, 55, 85, 95, 115, 125, 135, 65};
  const int lenghts[NUM_OF_CIRCLES] = {7, 7, 6, 5, 4, 4, 4, 3, 3, 1};
  uint32_t colors[NUM_OF_CIRCLES] = {0xF8AA00, 0xF8AA00, 0xF8AA00, 0xff7618,
                                     0xFF9400, 0xFF4000, 0xFF3000, 0xFF6400,
                                     0xFF3000};
  static uint prev_millis[NUM_OF_CIRCLES] = {0};
  // circle 0
  const int start_arc_0 = CIRCLES[0];
  const int end_arc_0 = CIRCLES[1];
  static int pixels_arc_0[7] = {0};  // active pixels nr
  // circle 1
  const int start_arc_1 = CIRCLES[0]+CIRCLES[1];
  const int end_arc_1 = CIRCLES[1]+CIRCLES[2];
  static int pixels_arc_1[7] = {0};  // nr
  // circle 2
  const int start_arc_2 = CIRCLES[1]+CIRCLES[2];
  const int end_arc_2 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3];
  static int pixels_arc_2[6] = {0};  // nr
  // circle 3
  const int start_arc_3 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3];
  const int end_arc_3 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4];
  static int pixels_arc_3[5] = {0};  // nr
  // circle 4
  const int start_arc_4 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4];
  const int end_arc_4 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4]+CIRCLES[5];
  static int pixels_arc_4[4] = {0};  // nr
  // circle 5
  const int start_arc_5 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]
                         +CIRCLES[4]+CIRCLES[5];
  const int end_arc_5 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]
                         +CIRCLES[4]+CIRCLES[5]+CIRCLES[6];
  static int pixels_arc_5[4] = {0};  // nr
  // circle 6
  const int start_arc_6 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]
                         +CIRCLES[4]+CIRCLES[5]+CIRCLES[6];
  const int end_arc_6 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]
                       +CIRCLES[4]+CIRCLES[5]+CIRCLES[6]+CIRCLES[7];
  static int pixels_arc_6[4] = {0};  // nr
  // circle 7
  const int start_arc_7 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4]
                         +CIRCLES[5]+CIRCLES[6]+CIRCLES[7];
  const int end_arc_7 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4]
                       +CIRCLES[5]+CIRCLES[6]+CIRCLES[7]+CIRCLES[8];
  static int pixels_arc_7[3] = {0};  // nr
  // circle 8
  const int start_arc_8 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4]
                         +CIRCLES[5]+CIRCLES[6]+CIRCLES[7]+CIRCLES[8];
  const int end_arc_8 = CIRCLES[1]+CIRCLES[2]+CIRCLES[3]+CIRCLES[4]
                       +CIRCLES[5]+CIRCLES[6]+CIRCLES[7]+CIRCLES[8]+CIRCLES[9];
  static int pixels_arc_8[3] = {0};  // nr

  if (initialize) {
    delay(2000);
    Serial.println("Init");
    initialize = false;
    // all LEDs off
    for (int i = 0; i < LED_COUNT; i++) {
      LED_colors[i] = 0x00;
    }
    // init arc 0
    for (int i = 0; i < lenghts[0]; i++) {
      pixels_arc_0[i] = start_arc_0 + i;
    }
    // init arc 1
    for (int i = 0; i < lenghts[1]; i++) {
      pixels_arc_1[i] = start_arc_1 + i;
    }
    // init arc 2
    for (int i = 0; i < lenghts[2]; i++) {
      pixels_arc_2[i] = start_arc_2 + i;
    }
    // init arc 3
    for (int i = 0; i < lenghts[3]; i++) {
      pixels_arc_3[i] = start_arc_3 + i;
    }
    // init arc 4
    for (int i = 0; i < lenghts[4]; i++) {
      pixels_arc_4[i] = start_arc_4 + i;
    }
    // init arc 5
    for (int i = 0; i < lenghts[5]; i++) {
      pixels_arc_5[i] = start_arc_5 + i;
    }
    // init arc 6
    for (int i = 0; i < lenghts[6]; i++) {
      pixels_arc_6[i] = start_arc_6 + i;
    }
    // init arc 7
    for (int i = 0; i < lenghts[7]; i++) {
      pixels_arc_7[i] = start_arc_7 + i;
    }
    // init arc 8
    for (int i = 0; i < lenghts[8]; i++) {
      pixels_arc_8[i] = start_arc_8 + i;
    }
  }

  // 0 update arc time
  if ((millis() - prev_millis[0]) >= delays[0]) {
    prev_millis[0] = millis();
    update[0] = true;
    // Serial.println("Time arc 1");
  }
  // 1 update arc 2 time
  if ((millis() - prev_millis[1]) >= delays[1]) {
    prev_millis[1] = millis();
    update[1] = true;
  }
  // 2 update arc 3 time
  if ((millis() - prev_millis[2]) >= delays[2]) {
    prev_millis[2] = millis();
    update[2] = true;
  }
  // 3 update arc 3 time
  if ((millis() - prev_millis[3]) >= delays[3]) {
    prev_millis[3] = millis();
    update[3] = true;
  }
  // 4 update arc 5 time
  if ((millis() - prev_millis[4]) >= delays[4]) {
    prev_millis[4] = millis();
    update[4] = true;
  }
  // 5 update arc 6 time
  if ((millis() - prev_millis[5]) >= delays[5]) {
    prev_millis[5] = millis();
    update[5] = true;
  }
  // 6 update arc 7 time
  if ((millis() - prev_millis[6]) >= delays[6]) {
    prev_millis[6] = millis();
    update[6] = true;
  }
  // 7 update arc 7 time
  if ((millis() - prev_millis[7]) >= delays[7]) {
    prev_millis[7] = millis();
    update[7] = true;
  }
  // 8 update arc 9 time
  if ((millis() - prev_millis[8]) >= delays[8]) {
    prev_millis[8] = millis();
    update[8] = true;
  }

  // update arc 0 pixels
  if (update[0]) {
    update[0] = false;
    colors_changed = true;

    // remove old pixel colors
    LED_colors[pixels_arc_0[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[0]; i++) {
      if (pixels_arc_0[i] == end_arc_0 -1) {
        pixels_arc_0[i] = start_arc_0;
      } else {
        pixels_arc_0[i] = pixels_arc_0[i] +1;
      }
    }

    // move colors
    for (int i = 0; i < lenghts[0]; i++) {
      LED_colors[pixels_arc_0[i]] = colors[0];
    }
  }

  // update arc 1 pixels
  if (update[1]) {
    update[1] = false;
    colors_changed = true;
    LED_colors[pixels_arc_1[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[1]; i++) {
      if (pixels_arc_1[i] == end_arc_1 -1) {
        pixels_arc_1[i] = start_arc_1;
      } else {
        pixels_arc_1[i] = pixels_arc_1[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[1]; i++) {
      LED_colors[pixels_arc_1[i]] = colors[1];
    }
  }

  // update arc 2 pixels
  if (update[2]) {
    update[2] = false;
    colors_changed = true;
    LED_colors[pixels_arc_2[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[2]; i++) {
      if (pixels_arc_2[i] == end_arc_2 -1) {
        pixels_arc_2[i] = start_arc_2;
      } else {
        pixels_arc_2[i] = pixels_arc_2[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[2]; i++) {
      LED_colors[pixels_arc_2[i]] = colors[2];
    }
  }

  // update arc 3 pixels
  if (update[3]) {
    update[3] = false;
    colors_changed = true;
    LED_colors[pixels_arc_3[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[3]; i++) {
      if (pixels_arc_3[i] == end_arc_3 -1) {
        pixels_arc_3[i] = start_arc_3;
      } else {
        pixels_arc_3[i] = pixels_arc_3[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[3]; i++) {
      LED_colors[pixels_arc_3[i]] = colors[3];
    }
  }

  // update arc 4 pixels
  if (update[4]) {
    update[4] = false;
    colors_changed = true;
    LED_colors[pixels_arc_4[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[4]; i++) {
      if (pixels_arc_4[i] == end_arc_4 -1) {
        pixels_arc_4[i] = start_arc_4;
      } else {
        pixels_arc_4[i] = pixels_arc_4[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[4]; i++) {
      LED_colors[pixels_arc_4[i]] = colors[4];
    }
  }

  // update arc 5 pixels
  if (update[5]) {
    update[5] = false;
    colors_changed = true;
    LED_colors[pixels_arc_5[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[5]; i++) {
      if (pixels_arc_5[i] == end_arc_5 -1) {
        pixels_arc_5[i] = start_arc_5;
      } else {
        pixels_arc_5[i] = pixels_arc_5[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[5]; i++) {
      LED_colors[pixels_arc_5[i]] = colors[5];
    }
  }

  // update arc 6 pixels
  if (update[6]) {
    update[6] = false;
    colors_changed = true;
    LED_colors[pixels_arc_6[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[6]; i++) {
      if (pixels_arc_6[i] == end_arc_6 -1) {
        pixels_arc_6[i] = start_arc_6;
      } else {
        pixels_arc_6[i] = pixels_arc_6[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[6]; i++) {
      LED_colors[pixels_arc_6[i]] = colors[6];
    }
  }

  // update arc 7 pixels
  if (update[7]) {
    update[7] = false;
    colors_changed = true;
    LED_colors[pixels_arc_7[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[7]; i++) {
      if (pixels_arc_7[i] == end_arc_7 -1) {
        pixels_arc_7[i] = start_arc_7;
      } else {
        pixels_arc_7[i] = pixels_arc_7[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[7]; i++) {
      LED_colors[pixels_arc_7[i]] = colors[7];
    }
  }

  // update arc 8 pixels
  if (update[8]) {
    update[8] = false;
    colors_changed = true;
    LED_colors[pixels_arc_8[0]] = 0x00;

    // move pixels
    for (int i = 0; i < lenghts[8]; i++) {
      if (pixels_arc_8[i] == end_arc_8 -1) {
        pixels_arc_8[i] = start_arc_8;
      } else {
        pixels_arc_8[i] = pixels_arc_8[i] +1;
      }
    }
    // move colors
    for (int i = 0; i < lenghts[8]; i++) {
      LED_colors[pixels_arc_8[i]] = colors[8];
    }
  }

  // ALL colors OUT
  if (colors_changed) {
    colors_changed = false;
    pixels.clear();
    for (int led = 0; led < LED_COUNT; led++) {
      pixels.setPixelColor(led, LED_colors[led]);
    }
    delay(1);
    pixels.show();
  }
}


/*
 *
 */

void alt_ylesse(int delay_val, uint32_t colour) {
  const int arc_count = 4;
  const int max_arc_size = 11;
  static int current_arc = 0;

  const int arc_sizes[arc_count] = {};

  // negatiivset ei loe
  const int arcs[arc_count][max_arc_size] = {
    {8, 9},
    {7, 10, 40, 41},
    {6, 11, 38, 43, 71, 74, 103, 104, -1,  -1,  -1},
    {6, 11, 38, 43, 70, 75, 102, 105, 128, 129, 138}
  };

  static uint32_t LED_colors[LED_COUNT] = {0};
  static bool colors_changed = false;
  static bool update = false;
  uint prev_millis = 0;

  // Time
  if ((millis() - prev_millis) >= delay_val) {
    prev_millis = millis();
    update = true;
  }

  // Change
  if (update) {
    update = false;
    colors_changed = true;

    if (current_arc >= arc_count) {
        current_arc = 0;
    }
    // eemalda vanad
    LED_colors[LED_COUNT] = {0};

    // lisa uued kaare ledid
    int led = 0;
    for (int i = 0; i < max_arc_size; i++) {
      led = arcs[current_arc][i];
      LED_colors[led] = colour;
    }
    
    current_arc++;
  }

  // ALL colors OUT
  if (colors_changed) {
    colors_changed = false;
    pixels.clear();
    for (int led = 0; led < LED_COUNT; led++) {
      pixels.setPixelColor(led, LED_colors[led]);
    }
    delay(1);
    pixels.show();
  }
}


/*
 *
 */

void breathing(int delay_val, uint32_t colour) {
  static uint start = 0;
  static uint end = 0;

  // Seest välja
  // Ringi algused
  for (uint i = 0; i < NUM_OF_CIRCLES; i++) {
    start += CIRCLES[i];
  }
  // Ringi lõppud
  for (uint i = 1; i <= NUM_OF_CIRCLES+1; i++) {
    end += CIRCLES[i];
  }

  for (uint i = NUM_OF_CIRCLES; i > 0; i--) {
    pixels.clear();

    for (int p = start; p < end; p++) {
      pixels.setPixelColor(p, colour);
    }

    pixels.show();
    delay(delay_val);
    start -= CIRCLES[i-1];
    end -= CIRCLES[i];
  }

}


/************************************************************************/
void setup() {
  Serial.begin(115200);

  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
}


void loop() {
  // strip.clear();  // all off

  // ringid_out_to_in(500);
  // ringid_in_to_out(100);
  // ringid_in_to_out(50, (pixels.Color(20, 70, 0)));
  // ringid_in_to_out(50, (pixels.Color(70, 20, 0)));
  // one_by_one(100);
  // tester(10);
  // hex_color();
  // bounce(20);
  // fade_all(40);
  // fade_chase();
  // sektor(50);
  // kaar(1, 5, 0xF8AA00, 75);


  // celestial_object();
  // ringid_in_to_out(100, 0xff7d00);

  //celestial_object();
  //ringid_in_to_out(100, 0xff7d00);

  alt_ylesse(100, 0xff7d00);

  // pixels.clear();

  // Fill along the length of the strip in various colors...
  // colorWipe(strip.Color(255, 0, 0), 50);  // Red
  // colorWipe(strip.Color(0, 255, 0), 50);  // Green
  // colorWipe(strip.Color(0, 0, 255), 50);  // Blue
  // rainbow(10);             // Flowing rainbow cycle along the whole strip
  // theaterChaseRainbow(50);  // Rainbow-enhanced theaterChase variant
}




