#include "leds.h"

void updateLeds()
{
    for (byte r = 0; r < 2; r++)
    {
        for (byte c = 0; c < 8; c++)
        {
            led.setPixelColor(swLedId[r][c], ledProfiles[activeProfile][r][c]);
        }
    }
}

void updateLeds_Case()
{
    for (byte l; l < 4; l++)
    {
        led.setPixelColor(caseLedId[l], caseLedProfiles[caseLedProfile][l]);
    }
}

void led_begin()
{
    led.begin();
    led.show();
    led.setBrightness(127);
    updateLeds_Case();
    updateLeds();
    led.show();
}