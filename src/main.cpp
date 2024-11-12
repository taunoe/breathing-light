/*
 * File:        main.cpp
 * Copyright    Tauno Erik
 * Started:     24.12.2023
 * Last edited: 12.11.2024
 * Project name:The Breathing Light
 * GitHub:      https://github.com/taunoe/breathing-light
 * 
 * Vaata:
 *  https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest_nodelay/strandtest_nodelay.ino
 * 
 * Hingamisharjutused:
 * - https://www.ph.ee/et/patsiendile-ja-kulastajale/patsiendi-infomaterjalid/enesehooldus/rahustav-hingamine
 * - https://peaasi.ee/votted-rahunemiseks/
 * - https://www.tlu.ee/meediavarav/blogid/pinge-leevendamise-votted-rahustav-hingamine
 * - https://www.youtube.com/watch?v=k4C9FECTCcI
 *
 * Diafragmaalhingamine:
 * - sisse 5-10 sekundit
 * - Välja 5-10 sekundit
 *
 * Hingamine hingamispeetusega:
 * - sisse
 * - hoida hinge kinni 3-5 sekundit
 * - välja
 * 
 * 
 * 
 * https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors
 * 
 * Hue colors:
 *   0 - red
 *  32 - orange
 *  64 - yellow
 *  96 - green
 * 128 - aqua
 * 160 - blue
 * 192 - purple
 * 224 - pink
 * 
 */

#include <Arduino.h>
#include "Tauno_rotary_encoder.h"  // Rotary Encoder
#include "Tauno_Display_Char.h"    // 7-segment LED
#include "FastLED.h"  // 3.7.0 does not work!

// https://arduino-pico.readthedocs.io/en/latest/eeprom.html
// https://github.com/MakerMatrix/RP2040_flash_programming/blob/main/RP2040_flash/RP2040_flash.ino
#include <EEPROM.h>
int program_address = 0;

//------------------------------------------------------------------
// Rotary Encoder
// pins
const int RE_SW_PIN  = 13;
const int RE_CLK_PIN = 15;
const int RE_DT_PIN  = 14;

// Rotary Encoder variables
int selected_program = 1;
int old_selected_program = selected_program;

int RE_state = 0;
int last_RE_state = 0;
uint32_t last_debounce_time = 0;
uint32_t debounce_delay = 50;
bool change = false;

// Declare Rotary Encoder object:
Tauno_Rotary_Encoder RE(RE_SW_PIN, RE_CLK_PIN, RE_DT_PIN);

//------------------------------------------------------------------
// 7-SEGMENT LED
// Pins
const int  A_PIN = 18;  // 1
const int  B_PIN = 19;  // 2
const int DP_PIN = 20;  // 3
const int  C_PIN = 21;  // 4
const int  D_PIN = 22;  // 5
const int  E_PIN = 26;  // 6
const int  G_PIN = 27;  // 7
const int  F_PIN = 28;  // 8

// Declare 7-segment LED object
Tauno_Display_Char Number(A_PIN, B_PIN, DP_PIN, C_PIN,
                          D_PIN, E_PIN, G_PIN, F_PIN);

//------------------------------------------------------------------
// Circular RGB LED display
const int LED_PIN = 6;

// Total number of LEDs:
const uint NUM_LEDS = 224;

// FastLED memory block:
CRGB leds[NUM_LEDS];

bool clear_display = false;  // Clear display from old program
int led_brightness = 50;     // 0-255

//---------------------------------------------------------------------------
// Circle 1: 0 - 34
const uint circle_1_num = 35;
int circle_1_leds[circle_1_num] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                                  13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                                  24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};

// Circle 2: 35 - 69
const uint circle_2_num = 35;
int circle_2_leds[circle_2_num] = {35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
                                   46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                                   57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
                                   68, 69};
// Circle 3: 70 - 104
const uint circle_3_num = 35;
int circle_3_leds[circle_3_num] = {70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                                   81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
                                   92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
                                   102, 103, 104};
// Circle 4: 105 - 132
const uint circle_4_num = 28;
int circle_4_leds[circle_4_num] = {105, 106, 107, 108, 109, 110, 111, 112, 113,
                                   114, 115, 116, 117, 118, 119, 120, 121, 122,
                                   123, 124, 125, 126, 127, 128, 129, 130, 131,
                                   132};
// Circle 5: 133 - 160
const uint circle_5_num = 28;
int circle_5_leds[circle_5_num] = {133, 134, 135, 136, 137, 138, 139, 140, 141,
                                   142, 143, 144, 145, 146, 147, 148, 149, 150,
                                   151, 152, 153, 154, 155, 156, 157, 158, 159,
                                   160};
// Circle 6: 161 - 181
const uint circle_6_num = 21;
int circle_6_leds[circle_6_num] = {161, 162, 163, 164, 165, 166, 167, 168, 169,
                                   170, 171, 172, 173, 174, 175, 176, 177, 178,
                                   179, 180, 181};
// Circle 7: 182 - 202
const uint circle_7_num = 21;
int circle_7_leds[circle_7_num] = {182, 183, 184, 185, 186, 187, 188, 189, 190,
                                   191, 192, 193, 194, 195, 196, 197, 198, 199,
                                   200, 201, 202};
// Circle 8: 203 - 216
const uint circle_8_num = 14;
int circle_8_leds[circle_8_num] = {203, 204, 205, 206, 207, 208, 209, 210, 211,
                                   212, 213, 214, 215, 216};
// Circle 9: 217 - 223
const uint circle_9_num =  7;
int circle_9_leds[circle_9_num] = {217, 218, 219, 220, 221, 222, 223};


//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void tauno_katsetus();
void tauno_rainbow(int wait);
void tauno_rainbow_circular(int wait);
void tauno_rainbow_circular_out(int wait);
void tauno_juggle();
void tauno_bpm();
void tauno_sinelon();
void tauno_confetti();
void all_off();
void fade_in(int led_array[], int num_of_LEDs, int speed, uint8_t hue);
void fade_out(int led_array[], int num_of_LEDs, int speed, uint8_t hue);
void LEDs_on(int led_array[], int num_of_LEDs, uint8_t hue);
void circles_in_out_1(int wait);
void circles_in_out_2(int wait);
void arches_bottom_to_up(int wait);


/*****************************************
 * Core 0 setup
 *****************************************/
void setup() {
  Serial.begin(115200);
  //while (!Serial) {}  // Suspends the program until serial is connected

  EEPROM.begin(256);

  // Read last saved program from flash memory:
  selected_program = EEPROM.read(program_address);
  old_selected_program = selected_program;

  Serial.print("selected_program: ");
  Serial.print(selected_program, DEC);
  Serial.print(" old: ");
  Serial.print(old_selected_program, DEC);
  Serial.println();

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(led_brightness);  // 0-255
}

/*****************************************
 * Core 1 setup
 *****************************************/
void setup1() {
  // Initialize rotary encoder
  if (!RE.begin()) {
    Serial.println("Error, failed to initialize Rotary Encoder!");
    // while (1) {}  // Stop pogram
  }
  // initialize 7-segment LED
  if (!Number.begin()) {
    Serial.println("Error, failed to initialize 7-segment LED!");
    // while (1) {}  // Stop pogram
  }
  // Number.test(50);  // Display all numbers and letters
}

/*****************************************
 * Core 0 loop
 *****************************************/
void loop() {
  // If is selected new program clear display
  if (clear_display) {
    all_off();
    clear_display = false;
  }

  // Display selected program
  switch (selected_program) {
    case 1:
      // Breathing technique 1
      circles_in_out_1(200);  // was 500
      break;
    case 2:
      circles_in_out_2(200);
      break;
    case 3:
      // 
      arches_bottom_to_up(80);
      break;
    case 4:
      //
      tauno_sinelon();
      break;
    case 5:
      tauno_rainbow_circular_out(20);
      break;
    case 6:
      tauno_juggle();
      break;
    case 7:
      tauno_rainbow(20);
      break;
    case 8:
      tauno_confetti();
      break;
    case 9:
      tauno_rainbow_circular(20);
      break;
    case 10:
      //
      break;
    case 11:
      // Wheel.rainbow(50);  // Kiire Jääb sisse
      break;
    case 12:
      // Wheel.rainbow(80);  // Aeglane Jääb sisse
      break;
    case 13:
      //
      break;
    case 14:
      //
      break;
    case 15:
      //
      break;
    case 16:
      //
      break;
    case 0:  // OFF mode
    default:
      all_off();  // Set all LEDs to off
      break;
  }
}

/*****************************************
 * Core 1 loop
 *****************************************/
void loop1() {
  static uint64_t num_start_time;
  uint64_t num_on_time = 1500;     // Time till 7-segment led stays on
  const int NUM_OF_PROGRAMS = 9;  // Max == 17


  // Turn the number off after some time
  if ((millis() - num_start_time) >= num_on_time) {
    Number.clear();  // off 7-segment

    // Read old
    int old_saved = EEPROM.read(program_address);
    // Save data if new program
    if (old_saved != selected_program) {
      EEPROM.write(program_address, selected_program);

      Serial.print("old_saved ");
      Serial.println(old_saved);
      Serial.print("selected_program ");
      Serial.println(selected_program);

      if (EEPROM.commit()) {
        Serial.println("EEPROM successfully committed");
      } else {
        Serial.println("ERROR! EEPROM commit failed");
      }
    }
  }

  // Read Rotary Encoder rotation direction
  int re_direction = RE.read();

  // Read Rotary Encoder rotation speed:
  uint16_t re_speed = RE.speed();

  // Read Rotary Encoder button:
  int button = RE.button();

  if (selected_program > NUM_OF_PROGRAMS) {
    selected_program = 0;
  }
  if (selected_program < 0) {
    selected_program = NUM_OF_PROGRAMS;
  }

  // If the Rotary Encoder is rotated:
  // CW = clockwise rotation
  // CCW = counterclockwise rotation
  if (re_direction == DIR_CW) {
    Serial.print(" Speed:");
    Serial.println(re_speed);
    selected_program++;
    Serial.print("Selected program ");
    Serial.println(selected_program);
    num_start_time = millis();
  } else if (re_direction == DIR_CCW) {
    Serial.print(" Speed:");
    Serial.println(re_speed);
    selected_program--;
    Serial.print("Selected program ");
    Serial.println(selected_program);
    num_start_time = millis();
  }

  // If is new selected program
  if (selected_program != old_selected_program) {
    old_selected_program = selected_program;
    Number.display(selected_program);
    num_start_time = millis();

    // Clear display from old program
    clear_display = true;
  }

  // If Rotary Encoder button is pressed:
  if (button) {
    Serial.println("Button!");
  }
}

//-----------------------------------------------------------------------------
// Custom Functions
//-----------------------------------------------------------------------------

void tauno_katsetus() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Blue;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(30);
  }
}

//
void tauno_rainbow(int wait) {
  static uint8_t initial_hue = 0;
  const uint8_t delta_hue = 7;
  const uint8_t hue_density = 1;

  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    fill_rainbow(leds, NUM_LEDS, initial_hue += hue_density, delta_hue);
    FastLED.show();
    prev_millis = millis();
  }
}

//
void tauno_rainbow_circular(int wait) {
  static uint8_t initial_hue = 0;
  const uint8_t delta_hue = 7;
  const uint8_t hue_density = 1;

  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    fill_rainbow_circular(leds, NUM_LEDS, initial_hue += hue_density, true);
    FastLED.show();
    prev_millis = millis();
  }
}

//
void tauno_rainbow_circular_out(int wait) {
  static uint8_t initial_hue = 0;
  const uint8_t delta_hue = 7;
  const uint8_t hue_density = 1;

  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    fill_rainbow_circular(leds, NUM_LEDS, initial_hue += hue_density, false);
    FastLED.show();
    prev_millis = millis();
  }
}

//
void tauno_juggle() {
  // eight colored dots, weaving in and out of sync with each other
  int wait = 250;
  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    static uint8_t dothue = 0;
    for (int i = 0; i < 2; i++) {
      leds[beatsin16(i+7, 0, NUM_LEDS-1)] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
    prev_millis = millis();
    FastLED.show();
  }
}


//
void tauno_bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  static uint8_t gHue = 0;
  uint8_t BeatsPerMinute = 12;

  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);

  int wait = 250;
  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    for (int i = 0; i < NUM_LEDS; i++) {  // 9948
      leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
    FastLED.show();
  }
}

//
void tauno_sinelon() {
  static uint8_t gHue = 70;
  // a colored dot sweeping back and forth, with fading trails
  int wait = 50;
  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beatsin16(13, 0, NUM_LEDS-1);
    leds[pos] += CHSV(gHue, 255, 192);
    FastLED.show();
    prev_millis = millis();
  }
}

//
void tauno_confetti() {
  static uint8_t gHue = 220;
  // random colored speckles that blink in and fade smoothly
  int wait = 100;
  static uint32_t prev_millis = 0;

  if ((millis() - prev_millis) >= wait) {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(gHue + random8(64), 200, 255);
    FastLED.show();
    prev_millis = millis();
  }
}

// Korraga põleb üks ring
void circles_in_out_1(int wait) {
  const int interval = wait;
  const int in_speed = 50;
  const int out_speed = 10;
  uint8_t hue = 70;  // Color
  static uint32_t prev_millis = 0;
  uint32_t currentMillis = millis();
  const int IN = 1;
  const int OUT = 0;
  static int direction = IN;
  static int counter = 0;

  if (currentMillis - prev_millis >= interval) {
    prev_millis = currentMillis;

    Serial.println(counter);

    switch (counter) {
      case 1:
        if (direction == OUT) {
          fade_in(circle_1_leds, circle_1_num, in_speed, hue);
          fade_out(circle_2_leds, circle_2_num, out_speed, hue);
        }
        break;
      case 2:
        fade_in(circle_2_leds, circle_2_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_1_leds, circle_1_num, out_speed, hue);
        } else {
          fade_out(circle_3_leds, circle_3_num, out_speed, hue);
        }
        break;
      case 3:
        fade_in(circle_3_leds, circle_3_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_2_leds, circle_2_num, out_speed, hue);
        } else {
          fade_out(circle_4_leds, circle_4_num, out_speed, hue);
        }
        break;
      case 4:
        fade_in(circle_4_leds, circle_4_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_3_leds, circle_3_num, out_speed, hue);
        } else {
          fade_out(circle_5_leds, circle_5_num, out_speed, hue);
        }
        break;
      case 5:
        fade_in(circle_5_leds, circle_5_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_4_leds, circle_4_num, out_speed, hue);
        } else {
          fade_out(circle_6_leds, circle_6_num, out_speed, hue);
        }
        break;
      case 6:
        fade_in(circle_6_leds, circle_6_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_5_leds, circle_5_num, out_speed, hue);
        } else {
          fade_out(circle_7_leds, circle_7_num, out_speed, hue);
        }
        break;
      case 7:
        fade_in(circle_7_leds, circle_7_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_6_leds, circle_6_num, out_speed, hue);
        } else {
          fade_out(circle_8_leds, circle_8_num, out_speed, hue);
        }
        break;
      case 8:
        fade_in(circle_8_leds, circle_8_num, in_speed, hue);
        if (direction == IN) {
          fade_out(circle_7_leds, circle_7_num, out_speed, hue);
        } else {
          fade_out(circle_9_leds, circle_9_num, out_speed, hue);
        }
        break;
      case 9:
        if (direction == IN) {
          fade_in(circle_9_leds, circle_9_num, in_speed, hue);
          fade_out(circle_8_leds, circle_8_num, out_speed, hue);
        }
        break;
      case  0:
      case 10:
        // Wait
        break;
      default:
        break;
    }

    if (direction == IN) {
      counter++;
    }

    if (direction == OUT) {
      counter--;
    }

    if (counter >= 11) {
      counter = 10;
      direction = OUT;
    }

    if (counter <= -1) {
      counter = 0;
      direction = IN;
    }
  }
}


// Set all LEDs to off
void all_off() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Fade In an array of LEDs
void fade_in(int led_array[], int num_of_LEDs, int speed, uint8_t hue) {
  // Serial.print("fade in start ");
  // Variables to control the fading
  static uint8_t brightness = 0;
  static int fade_amount = 5;
  static uint32_t prev_millis = 0;
  const int fade_interval = speed;

  for (int i = 0; i < 256; i += fade_amount) {
    uint32_t current_millis = millis();

    // Check if it's time to update the brightness
    if (current_millis - prev_millis >= fade_interval) {
      // Save the last update time
      prev_millis = current_millis;

      // Set the brightness of the specified LEDs
      for (int j = 0; j < num_of_LEDs; j++) {
        int index = led_array[j];
        leds[index] = CHSV(hue, 255, i);
        // Serial.print(index);
        // Serial.print(" ");
      }
      // FastLED.show();
    }
    FastLED.show();
  }
  // Serial.print("fade in end!\n");
}

// Fade Out an array of LEDs
void fade_out(int led_array[], int num_of_LEDs, int speed, uint8_t hue) {
  // Serial.print("fade out start ");
  // Variables to control the fading
  static uint8_t brightness = 0;
  static int fade_amount = 5;
  static uint32_t prev_millis = 0;
  const int fade_interval = speed;

  for (int i = 255; i >= 0; i -= fade_amount) {
    uint32_t current_millis = millis();

    // Check if it's time to update the brightness
    if (current_millis - prev_millis >= fade_interval) {
      // Save the last update time
      prev_millis = current_millis;

      // Set the brightness of the specified LEDs
      for (int j = 0; j < num_of_LEDs; j++) {
        int index = led_array[j];
        leds[index] = CHSV(hue, 255, i);
        // Serial.print(index);
        // Serial.print(" ");
      }
      // FastLED.show();
    }
    FastLED.show();
  }
  // Serial.print("fade out end!\n");
}


// Sisemus põleb välja liikudes
void circles_in_out_2(int wait) {
  const uint32_t interval = wait;
  const int in_speed = 50;
  const int out_speed = 50;
  uint8_t hue = 70;  // Color
  static uint32_t prev_millis = 0;
  uint32_t current_millis = millis();
  const int IN = 1;
  const int OUT = 0;
  static int direction = OUT;
  static int counter = 10;

  // Most inner circle is always on
  LEDs_on(circle_9_leds, circle_9_num, hue);

  if (current_millis - prev_millis >= interval) {
    prev_millis = current_millis;

    // Serial.println(counter);
    switch (counter) {
      case 1:
        if (direction == OUT) {
          fade_in(circle_1_leds, circle_1_num, in_speed, hue);
          // fade_out(circle_2_leds, circle_2_num, out_speed, hue);
        }
        break;
      case 2:
        if (direction == IN) {
          fade_out(circle_1_leds, circle_1_num, out_speed, hue);
        } else {
          fade_in(circle_2_leds, circle_2_num, in_speed, hue);
        }
        break;
      case 3:
        if (direction == IN) {
          fade_out(circle_2_leds, circle_2_num, out_speed, hue);
        } else {
          fade_in(circle_3_leds, circle_3_num, in_speed, hue);
        }
        break;
      case 4:
        if (direction == IN) {
          fade_out(circle_3_leds, circle_3_num, out_speed, hue);
        } else {
          fade_in(circle_4_leds, circle_4_num, in_speed, hue);
        }
        break;
      case 5:
        if (direction == IN) {
          fade_out(circle_4_leds, circle_4_num, out_speed, hue);
        } else {
          fade_in(circle_5_leds, circle_5_num, in_speed, hue);
        }
        break;
      case 6:
        if (direction == IN) {
          fade_out(circle_5_leds, circle_5_num, out_speed, hue);
        } else {
          fade_in(circle_6_leds, circle_6_num, in_speed, hue);
        }
        break;
      case 7:
        if (direction == IN) {
          fade_out(circle_6_leds, circle_6_num, out_speed, hue);
        } else {
          fade_in(circle_7_leds, circle_7_num, in_speed, hue);
        }
        break;
      case 8:
        if (direction == IN) {
          fade_out(circle_7_leds, circle_7_num, out_speed, hue);
        } else {
          fade_in(circle_8_leds, circle_8_num, in_speed, hue);
        }
        break;
      case 9:
        if (direction == IN) {
          fade_out(circle_8_leds, circle_8_num, out_speed, hue);
        }

        break;
      case  0:
      case 10:
        // Wait
        break;
      default:
        break;
    }

    if (direction == IN) {
      counter++;
    }

    if (direction == OUT) {
      counter--;
    }

    if (counter >= 11) {
      counter = 10;
      direction = OUT;
    }

    if (counter <= -1) {
      counter = 0;
      direction = IN;
    }
  }
}

//------------------------------------------------------------------------------
// Turn ON array off LEDs,
// without delay
void LEDs_on(int led_array[], int num_of_LEDs, uint8_t hue) {
  for (int j = 0; j < num_of_LEDs; j++) {
    int index = led_array[j];
    leds[index] = CHSV(hue, 255, 255);
  }
  FastLED.show();
}

//------------------------------------------------------------------------------
void arches_bottom_to_up(int wait) {
  uint8_t hue = 32;  // Color
  const uint32_t interval = wait;
  static uint32_t prev_millis = 0;
  uint32_t current_millis = millis();
  static int counter = 0;
  const int UP = 1;
  const int DOWN = 0;
  static int direction = UP;
  int up_speed = 50;
  int down_speed = 50;
  const int cases_num = 16;

  // LED groups
  int leds_1_size = 3;
  int leds_1[leds_1_size] = {26, 61, 96};

  int leds_size = 6;
  int leds_2[leds_size] = {25, 27, 60, 62, 95, 97};
  int leds_3[leds_size] = {24, 28, 59, 63, 94, 98};
  int leds_4[leds_size] = {23, 29, 58, 64, 93, 99};
  int leds_5[leds_size] = {22, 30, 57, 65, 92, 100};
  int leds_6[leds_size] = {21, 31, 56, 66, 91, 101};
  int leds_7[leds_size] = {20, 32, 55, 67, 90, 102};
  int leds_8[leds_size] = {19, 33, 54, 68, 89, 103};
  int leds_9[leds_size] = {18, 34, 53, 69, 88, 104};
  int leds_10[leds_size] = {17, 0, 52, 35, 87, 70};
  int leds_11[leds_size] = {16, 1, 51, 36, 86, 71};
  int leds_12[leds_size] = {15, 2, 50, 37, 85, 72};
  int leds_13[leds_size] = {14, 3, 49, 38, 84, 73};
  int leds_14[leds_size] = {13, 4, 48, 39, 83, 74};
  int leds_15[leds_size] = {12, 5, 47, 40, 82, 75};
  int leds_16[leds_size] = {11, 6, 46, 41, 81, 76};

  // Center is always ON
  LEDs_on(circle_9_leds, circle_9_num, hue);

  if (current_millis - prev_millis >= interval) {
    prev_millis = current_millis;

    Serial.print("counter: ");
    Serial.print(counter);
    if (direction == UP) {
      Serial.print(" UP\n");
    } else {
      Serial.print(" DOWN\n");
    }

    switch (counter) {
      case 1:
        if (direction == UP) {
          fade_in(leds_1, leds_1_size, up_speed, hue);
        } else {
          fade_out(leds_1, leds_1_size, up_speed, hue);
        }
        break;
      case 2:
        if (direction == UP) {
          fade_in(leds_2, leds_size, up_speed, hue);
        } else {
          fade_out(leds_2, leds_size, up_speed, hue);
        }
        break;
      case 3:
        if (direction == UP) {
          fade_in(leds_3, leds_size, up_speed, hue);
        } else {
          fade_out(leds_3, leds_size, up_speed, hue);
        }
        break;
      case 4:
        if (direction == UP) {
          fade_in(leds_4, leds_size, up_speed, hue);
        } else {
          fade_out(leds_4, leds_size, up_speed, hue);
        }
        break;
      case 5:
        if (direction == UP) {
          fade_in(leds_5, leds_size, up_speed, hue);
        } else {
          fade_out(leds_5, leds_size, up_speed, hue);
        }
        break;
      case 6:
        if (direction == UP) {
          fade_in(leds_6, leds_size, up_speed, hue);
        } else {
          fade_out(leds_6, leds_size, up_speed, hue);
        }
        break;
      case 7:
        if (direction == UP) {
          fade_in(leds_7, leds_size, up_speed, hue);
        } else {
          fade_out(leds_7, leds_size, up_speed, hue);
        }
        break;
      case 8:
        if (direction == UP) {
          fade_in(leds_8, leds_size, up_speed, hue);
        } else {
          fade_out(leds_8, leds_size, up_speed, hue);
        }
        break;
      case 9:
        if (direction == UP) {
          fade_in(leds_9, leds_size, up_speed, hue);
        } else {
          fade_out(leds_9, leds_size, up_speed, hue);
        }
        break;
      case 10:
        if (direction == UP) {
          fade_in(leds_10, leds_size, up_speed, hue);
        } else {
          fade_out(leds_10, leds_size, up_speed, hue);
        }
        break;
      case 11:
        if (direction == UP) {
          fade_in(leds_11, leds_size, up_speed, hue);
        } else {
          fade_out(leds_11, leds_size, up_speed, hue);
        }
        break;
      case 12:
        if (direction == UP) {
          fade_in(leds_12, leds_size, up_speed, hue);
        } else {
          fade_out(leds_12, leds_size, up_speed, hue);
        }
        break;
      case 13:
        if (direction == UP) {
          fade_in(leds_13, leds_size, up_speed, hue);
        } else {
          fade_out(leds_13, leds_size, up_speed, hue);
        }
        break;
      case 14:
        if (direction == UP) {
          fade_in(leds_14, leds_size, up_speed, hue);
        } else {
          fade_out(leds_14, leds_size, up_speed, hue);
        }
        break;
      case 15:
        if (direction == UP) {
          fade_in(leds_15, leds_size, up_speed, hue);
        } else {
          fade_out(leds_15, leds_size, up_speed, hue);
        }
        break;
      case 16:
        if (direction == UP) {
          fade_in(leds_16, leds_size, up_speed, hue);
        } else {
          fade_out(leds_16, leds_size, up_speed, hue);
        }
        break;
    default:
      break;
    }

    if (direction == UP) { counter++; }
    if (direction == DOWN) { counter--; }

    // Highest UP point
    if (counter >= cases_num + 1) {
      counter = cases_num;
      direction = DOWN;
    }

    // Lowest DOWN point
    if (counter <= -1) {
      counter = 0;
      direction = UP;
    }
  }
}



