#include<Wire.h>
#include "RTClib.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE

void printTime(int h, int m, int s);


RTC_DS1307 RTC;

#define DATA_PIN 2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 11
#define BRIGHTNESS 15
#define FRAMES_PER_SECOND 120

CRGB leds[NUM_LEDS];
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup () {

  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Wire.begin();

  RTC.begin(); // load the time from your computer.

  if (! RTC.isrunning()) {

    Serial.println("RTC is NOT running!");// This will reflect the time that your sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

}


void loop () {

  //LEDs
  //Rainbow();

  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  // do some periodic updates
  EVERY_N_MILLISECONDS( 100 ) {
    gHue++;  // slowly cycle the "base color" through the
  }


  //TIME
  DateTime now = RTC.now();
  int curr_min = now.minute();
  int curr_hour = now.hour();

  printTime(curr_min, curr_hour);

  //SetLEDsOfCurrentTime(curr_min, curr_hour);
  SetLEDsOfCurrentTime(53, 17);
  
  if (curr_min == 0) RainbowAnimation(); 
  delay(10000);
}

void printTime(int m, int h) {
  Serial.print(h, DEC);
  Serial.print(':');
  Serial.print(m, DEC);
  Serial.println();
}

void RainbowAnimation()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 15);

}

void SetLEDsOfCurrentTime(int m, int h) {
  leds[4] = CRGB::Red;
  byte min = byte(m);
  byte hour = byte(h);
  if (hour > 12) hour -= 12;
  Serial.println("Time:");
  Serial.println(hour, BIN);
  Serial.println(min, BIN);

  for (int i = 0; i <= 3 ; i++) {
    byte bhour = hour;
    Serial.println("Read Bit from Clock");
    Serial.println(i);
    if (bitRead(bhour, i))
      Serial.println("LED was set");
    else
      Serial.println("LED was not set");

    if (bitRead(bhour, i))leds[3-i] = CRGB::Yellow;
    else leds[3-i] = CRGB::Black;
  }

  for (int u = 0; u <= 5 ; u++) {
    byte bmin = min;
    Serial.println("Read Bit from Clock");
    Serial.println(u + 5);
    if (bitRead(bmin, u))
      Serial.println("LED was set");
    else
      Serial.println("LED was not set");

    if (bitRead(bmin, u))leds[10 - u] = CRGB::Cyan;
    else leds[10 - u] = CRGB::Black;
  }
  delay(500);
}
