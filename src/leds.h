#ifndef LEDS
#define LEDS
#include <Adafruit_NeoPixel.h>
#include "profiles.h"

#define PROFILES 7
#define LED_PIN 11
#define LED_COUNT 20

byte caseLedProfile;

Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void led_begin();
void updateLeds();

const uint32_t red = led.gamma32(led.Color(250, 69, 0));
const uint32_t pink = led.gamma32(led.Color(255, 38, 196));
const uint32_t yellow = led.gamma32(led.Color(255, 204, 61));
const uint32_t blue = led.gamma32(led.Color(1, 143, 235));
const uint32_t blue4 = led.gamma32(led.Color(37, 100, 255));
const uint32_t turq = led.gamma32(led.Color(203, 253, 247));
const uint32_t turq1 = led.gamma32(led.Color(114, 239, 221));
const uint32_t turq2 = led.gamma32(led.Color(142, 177, 173));
const uint32_t turq3 = led.gamma32(led.Color(63, 86, 78));
const uint32_t orange = led.gamma32(led.Color(251, 163, 26));
const uint32_t green = led.gamma32(led.Color(30, 215, 96));
const uint32_t green2 = led.gamma32(led.Color(115, 206, 115));
const uint32_t grey = led.gamma32(led.Color(205, 205, 205));
const uint32_t white = led.gamma32(led.Color(245, 245, 245));
const uint32_t black = led.Color(0, 0, 0);

byte swLedId[3][8] =
    {
        {5, 3, 1, 6, 9, 12, 14, 17},
        {4, 2, 0, 8, 11, 13, 15, 19}};

byte caseLedId[4] = {10, 7, 16, 18};

uint32_t caseLedProfiles[3][4] =
    {
        {pink, turq1, pink, turq1},
        {white, white, white, white},
        {black, black, black, black},
};

uint32_t ledProfiles[PROFILES][2][8] =
    {
        {// Desktop
         {red, turq3, blue, turq2, black, black, green, red},
         {yellow, blue4, turq3, black, orange, black, green2, orange}},
};

#endif