#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "src/switches.h"

// LEDs
#define LED_PIN 11
#define LED_COUNT 20
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// TFT Screen
#define TFT_DC 12
#define TFT_RST 4
#define TFT_SCK 9
#define TFT_SDA 10
#define TFT_BLK 6
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_DC, TFT_SDA, TFT_SCK, TFT_RST);

// I2C Switches
#define SW_INT 8 // Arduino PCint pin
volatile bool SW_awakenByInterrupt;

Profiles activeProfile;

void setup()
{
    pinMode(SW_INT, INPUT_PULLUP);

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT4);

    sw_begin();

    activeProfile = Desktop;
}

void loop()
{
    handleMatrix(activeProfile);
    handleSwitches(activeProfile, SW_awakenByInterrupt);
}

ISR(PCINT0_vect)
{
    SW_awakenByInterrupt = true;
}