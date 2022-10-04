#include "switches.h"
#include <Adafruit_MCP23X17.h>

// mcp 23017 over I2C
#define SW_ADDR 0x20
Adafruit_MCP23X17 sw;

// Independent switches
#define SW_15 5
#define SW_16 6
const byte tSwitches[2] = {SW_15, SW_16}; // pin 26,27
#define R_SW_1 4
#define R_SW_2 7
#define R_SW_3 3
#define R_SW_4 2
const byte rSwitches[4] = {R_SW_1, R_SW_2, R_SW_3, R_SW_4}; // pin 25,28,24,23

// Matrix
#define ROWS 2
#define COLS 7
const byte rowPins[ROWS] = {9, 10};                    // pin 2,3
const byte colPins[COLS] = {11, 14, 15, 8, 12, 13, 0}; // pin 4,7,8,1,5,6,21

// Key state
/*
 * Switches' bitMap layout - 7 -> 0
 * SW13 | SW11 | SW9  | SW7 | SW5 | SW3 | SW1
 * SW14 | SW12 | SW10 | SW8 | SW6 | SW4 | SW2
 */
byte bitMap[2];
enum State
{
    idle,
    pressed,
    hold,
    released
};
State keyStates[3][7] = {idle};

// Timers
unsigned long startTime;
unsigned long debounceTime = 10;
unsigned long holdTime = 500;
unsigned long holdTimer;

////////////////////////////////////////////////////////////////////////////////////////////////////////

void sw_begin()
{
    sw.begin_I2C(SW_ADDR);

    // init matrix
    for (byte r = 0; r < ROWS; r++)
    {
        sw.pinMode(rowPins[r], INPUT_PULLUP);
    }
    for (byte c = 0; c < COLS; c++)
    {
        sw.pinMode(colPins[c], OUTPUT);
    }

    sw.setupInterrupts(false, false, LOW);

    // init interrupt on change for R_SW_# and SW_15/16
    for (byte s = 0; s < 4; s++)
    {
        sw.pinMode(rSwitches[s], INPUT);
        sw.setupInterruptPin(rSwitches[s], CHANGE);
    }
    for (byte s = 0; s < 2; s++)
    {
        sw.pinMode(tSwitches[s], INPUT);
        sw.setupInterruptPin(tSwitches[s], CHANGE);
    }

    Keyboard.begin();

    startTime = 0;
}

// pulse each column and store ALL pressed keys in bitmap
void scanKeys()
{
    for (byte c = 0; c < COLS; c++)
    {
        sw.digitalWrite(colPins[c], LOW);

        for (byte r = 0; r < ROWS; r++)
        {
            bitWrite(bitMap[r], c, !sw.digitalRead(rowPins[r])); // keypress is active low so invert to high.
        }

        sw.digitalWrite(colPins[c], HIGH);
    }
}

bool handleSwitch(bool closed, const byte &c, const byte &r)
{
    bool changed = false;

    switch (keyStates[r][c])
    {
    case idle:
        if (closed)
        {
            keyStates[r][c] = pressed;
            holdTimer = millis();
            changed = true;
        }
        break;

    case pressed:
        if ((millis() - holdTimer) > holdTime)
        {
            keyStates[r][c] = hold;
            changed = true;
        }
        else if (closed)
        {
            keyStates[r][c] = released;
            changed = true;
        }
        break;

    case hold:
        if (!closed)
        {
            keyStates[r][c] = released;
            changed = true;
        }
        break;

    case released:
        keyStates[r][c] = idle;
        break;
    }

    return changed;
}

bool updateStates()
{
    bool anyActivity = false;

    for (byte c = 0; c < COLS; c++)
    {
        for (byte r = 0; r < ROWS; r++)
        {
            anyActivity = handleSwitch(bitRead(bitMap[r], c), c, r);
        }
    }

    return anyActivity;
}

bool getKeys()
{
    bool keyActivity = false;

    // Limit how often the keypad is scanned. It makes the loop() faster
    if ((millis() - startTime) > debounceTime)
    {
        scanKeys();
        keyActivity = updateStates();
        startTime = millis();
    }

    return keyActivity;
}

// REM it might be better to include everything in scanKeys or to filter keyStates
void handleMatrix(Profiles &profile)
{
    if (getKeys())
    {
        for (byte c = 0; c < COLS; c++)
        {
            for (byte r = 0; r < ROWS; r++)
            {
                switch (keyStates[r][c])
                {
                case idle:
                    break;

                case pressed:
                    for (byte k = 0; k < MAX_MACRO; k++)
                    {
                        Keyboard.press(macroBase[profile][r][c][k]);
                    }
                    Keyboard.releaseAll(); // maybe in release? It wouldn't allow toggles
                    break;

                case hold:
                    for (byte k = 0; k < MAX_MACRO; k++)
                    {
                        Keyboard.press(macroHold[profile][r][c][k]);
                    }
                    Keyboard.releaseAll(); // maybe in release? It wouldn't allow toggle
                    break;

                case released:
                    /* code */
                    break;

                default:
                    break;
                }
            }
        }
    }
}

void handleSwitches(Profiles &profile, volatile bool &SW_awakenByInterrupt)
{
    if (SW_awakenByInterrupt)
    {
        switch (sw.getLastInterruptPin())
        {
        case R_SW_1:
            handleSwitch(sw.digitalRead(R_SW_1), 1, 3);
            break;

        case R_SW_2:
            handleSwitch(sw.digitalRead(R_SW_2), 2, 3);
            break;

        case R_SW_3:
            handleSwitch(sw.digitalRead(R_SW_3), 3, 3);
            break;

        case R_SW_4:
            handleSwitch(sw.digitalRead(R_SW_4), 4, 3);
            break;

        case SW_15:
            handleSwitch(sw.digitalRead(SW_15), 5, 3);
            break;

        case SW_16:
            handleSwitch(sw.digitalRead(SW_16), 6, 3);
            break;

        default:
            break;
        }

        SW_awakenByInterrupt = false;
    }
}