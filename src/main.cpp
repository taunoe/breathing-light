/*
 * File:        main.cpp
 * Copyright    Tauno Erik
 * Started:     24.12.2023
 * Last edited: 19.05.2024
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
 */

#include <Arduino.h>
#include "Tauno_rotary_encoder.h"  // Rotary Encoder
#include "Tauno_Display_Char.h"    // 7-segment LED
#include "Tauno_RGBLED_Wheel.h"    // RGB LEDs


// Rotary Encoder pins
const int RE_SW_PIN  = 13;
const int RE_CLK_PIN = 15;
const int RE_DT_PIN  = 14;

int selected_program = 0;
int RE_state = 0;
int last_RE_state = 0;
uint32_t last_debounce_time = 0;
uint32_t debounce_delay = 50;
bool change = false;

// Declare Rotary Encoder object:
Tauno_Rotary_Encoder RE(RE_SW_PIN, RE_CLK_PIN, RE_DT_PIN);


// 7-SEGMENT LED number
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


// RGB LED strip
const int LED_PIN = 6;

// Number of LEDs in a circle,
// from outside to inside
const uint CIRCLE_1_NUM = 35;
const uint CIRCLE_2_NUM = 35;
const uint CIRCLE_3_NUM = 35;
const uint CIRCLE_4_NUM = 28;
const uint CIRCLE_5_NUM = 28;
const uint CIRCLE_6_NUM = 21;
const uint CIRCLE_7_NUM = 21;
const uint CIRCLE_8_NUM = 14;
const uint CIRCLE_9_NUM = 7;

const uint NUM_OF_CIRCLES = 9;

const uint CIRCLES[NUM_OF_CIRCLES + 1] = {
  0,
  CIRCLE_1_NUM, CIRCLE_2_NUM,
  CIRCLE_3_NUM, CIRCLE_4_NUM,
  CIRCLE_5_NUM, CIRCLE_6_NUM,
  CIRCLE_7_NUM, CIRCLE_8_NUM,
  CIRCLE_9_NUM};

// Kokku LEDe
const uint LED_COUNT = CIRCLE_1_NUM + CIRCLE_2_NUM + CIRCLE_3_NUM
                     + CIRCLE_4_NUM + CIRCLE_5_NUM + CIRCLE_6_NUM
                     + CIRCLE_7_NUM + CIRCLE_8_NUM + CIRCLE_9_NUM;

// Declare RGB LED object
Tauno_RGBLED_Wheel Wheel(LED_PIN);


/*****************************************
 * Core 0 setup
 *****************************************/
void setup() {
  Serial.begin(115200);
  Wheel.begin();
}

/*****************************************
 * Core 1 setup
 *****************************************/
void setup1() {
  RE.begin();
  Number.begin();    // 7-segment led pins
  Number.test(200);  // Display all numbers and letters
}

/*****************************************
 * Core 0 loop
 *****************************************/
void loop() {
  delay(1);  // !! Must be, otherwise errors!!

  switch (selected_program) {  // peab loopima kogu aeg!
    case 1:  // Breathing technique 1
      Wheel.all_on(0x000725);
      break;
    case 2:
      Wheel.all_on(0x000c3e);
      break;
    case 3:
      Wheel.all_on(0x001159);
      break;
    case 4:
      Wheel.all_on(0x021875);
      break;
    case 5:
      Wheel.all_on(0x011c91);
      break;
    case 6:
      Wheel.all_on(0x0020a9);
      break;
    case 7:
      Wheel.all_on(0x0226c2);
      break;
    case 8:
      Wheel.all_on(0x022ad9);
      break;
    case 9:
      Wheel.all_on(0x022eed);
      break;
    case 10:
      Wheel.all_on(0x0130fd);
      break;
    case 11:
      Wheel.all_on(0x00fca6);
      break;
    case 12:
      Wheel.all_on(0xd20792);
      break;
    case 13:
      Wheel.all_on(0xd2d007);
      break;
    case 14:
      Wheel.all_on(0xd24f07);
      break;
    case 15:
      Wheel.all_on(0x3c1aba);
      break;
    case 16:
      Wheel.all_on(0x0131fe);
      break;
    case 0:  // OFF mode
    default:
      Wheel.all_off();
      // Sleep?
      break;
  }
}

/*****************************************
 * Core 1 loop
 *****************************************/
void loop1() {
  static int old_selected_program;
  static uint64_t num_start_time;
  uint64_t num_on_time = 1500;
  const int NUM_OF_PROGRAMS = 17;

  // Turn number off after some time
  if ((millis() - num_start_time) >= num_on_time) {
    Number.clear();  // off
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

  // If Rotary Encoder is rotated:
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

  if (selected_program != old_selected_program) {
    old_selected_program = selected_program;
    Number.display(selected_program);
    num_start_time = millis();
  }

  // If Rotary Encoder button is pressed:
  if (button) {
    Serial.println("Button!");
  }
}
