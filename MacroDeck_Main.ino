#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MCP23X08.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "src/encoders.h"

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

// Switch matrix
unsigned long loopCount;
unsigned long startTime;

void setup()
{
    re_begin();

    loopCount = 0;
    startTime = millis();
}

void loop()
{
    checkEncoders();
}