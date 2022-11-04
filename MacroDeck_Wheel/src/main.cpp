#include <Adafruit_NeoPixel.h>

#include <encoders.h>
#include <switches.h>

// LEDs
#define LED_PIN 11
#define LED_COUNT 20
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    sw_begin();
    re_begin();

    activeProfile = Desktop;
}

void loop()
{
    handleMatrix();
    handleEncoders();
    handleSwitches();
}

ISR(PCINT0_vect)
{
    SW_awakenByInterrupt = true;
}