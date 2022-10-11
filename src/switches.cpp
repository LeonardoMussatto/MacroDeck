#include "switches.h"
#include "profiles.h"
#include "leds.h"
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
byte modRow = 0;
byte modCol = 0;

// Timers
unsigned long startTime;
const int debounceTime = 10;
const int shortPress = 500;
const int longPress = 1000;
unsigned long holdTimer;

////////////////////////////////////////////////////////////////////////////////////////////////////////

// ?? Is pressed called before hold? it would lead to sending two commands...
// REM handle switch might be able to handle matrix switches too

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

// Pulse each column and store ALL pressed keys in bitmap
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

bool handleState(bool closed, const byte &c, const byte &r)
{
    bool changed = false;

    switch (keyStates[r][c])
    {
    case idle:
        if (closed)
        {
            keyStates[r][c] = pressed;
            holdTimer = millis();
            return changed = true;
        }
        break;

    case pressed:
        if ((millis() - holdTimer) > shortPress)
        {
            keyStates[r][c] = hold;
            return changed = true;
        }
        else if (!closed)
        {
            keyStates[r][c] = released;
            return changed = true;
        }
        break;

    case hold:
        if ((millis() - holdTimer) > longPress)
        {
            keyStates[r][c] = longHold;
            return changed = true;
        }
        else if (!closed)
        {
            keyStates[r][c] = released;
            return changed = true;
        }
        break;

    case longHold:
        if (!closed)
        {
            keyStates[r][c] = released;
            return changed = true;
        }
        break;

    case released:
        keyStates[r][c] = idle;
        break;
    }
}

bool updateStates()
{
    bool anyActivity = false;

    for (byte c = 0; c < COLS; c++)
    {
        for (byte r = 0; r < ROWS; r++)
        {
            anyActivity = handleState(bitRead(bitMap[r], c), c, r);
        }
    }

    return anyActivity;
}

bool getKeys()
{
    bool keyActivity = false;

    // Limit how often the matrix is scanned. It makes the loop() faster
    if ((millis() - startTime) > debounceTime)
    {
        scanKeys();
        keyActivity = updateStates();
        startTime = millis();
    }

    return keyActivity;
}

// bool findMod(byte *row, byte *col)
// {
//     for (byte r = 0; r < 2; r++)
//     {
//         for (byte b = 0; b < 8; b++)
//         {
//             if (bitRead(swHold[r], b))
//             {
//                 *row = r;
//                 *col = b;
//                 return true;
//             }
//         }
//     }
// }

void selectProfile(byte &modRow, byte &modCol)
{
    if (bitRead(swHold[modRow], modCol))
    {
        switch (modCol)
        {
        case 0:
            if (!modRow)
            {
                /* code */
            }
            else
            {
                if (activeProfile == Desktop)
                    activeProfile = File_Explorer;
            }
            break;

        case 1:
            if (!modRow)
            {
                if (activeProfile == Desktop)
                    activeProfile = DaVinci_Edit;
            }
            else
            {
                if (activeProfile == Desktop)
                    activeProfile = Coding;
            }
            break;

        case 2:
            if (!modRow)
            {
                if (activeProfile == Desktop)
                    activeProfile = Firefox;
            }
            else
            {
                if (activeProfile == Desktop)
                    activeProfile = Reaper;
            }
            break;

        case 3:
            if (!modRow)
            {
                switch (activeProfile)
                {
                case Desktop:
                    activeProfile = Games;
                    break;

                case Graphic:
                    activeProfile = SketchUp;
                    break;

                case Coding:
                    activeProfile = KiCad;
                    break;

                default:
                    break;
                }
            }
            else
            {
                if (activeProfile == Graphic)
                    activeProfile = Fusion;
            }
            break;

        case 4:
            if (!modRow)
            {
                if (activeProfile == Graphic)
                    activeProfile = Cinema4D;
            }
            else
            {
                switch (activeProfile)
                {
                case Desktop:
                    activeProfile = Graphic;
                    break;

                case Graphic:
                    activeProfile = Blender;
                    break;

                default:
                    break;
                }
            }
            break;

        case 5:
            if (!modRow)
            {
                if (activeProfile == Graphic)
                    activeProfile = Unity;
            }
            else
            {
                if (activeProfile == Graphic)
                    activeProfile = Unreal;
            }
            break;

        case 6:
            if (!modRow)
            {
                if (activeProfile == Graphic)
                    activeProfile = TouchDesigner;
            }
            else
            {
                /*code*/
            }
            break;

        default:
            break;
        }
    }
    else
    {
        switch (modCol)
        {
        case 0:
            if (!modRow)
            {
                /* code */
            }
            else
            {
                if (activeProfile == File_Explorer)
                    activeProfile = Desktop;
            }
            break;

        case 1:
            if (!modRow)
            {
                if (activeProfile == DaVinci_Edit)
                    activeProfile = Desktop;
            }
            else
            {
                if (activeProfile == Coding)
                    activeProfile = Desktop;
            }
            break;

        case 2:
            if (!modRow)
            {
                if (activeProfile == Firefox)
                    activeProfile = Desktop;
            }
            else
            {
                if (activeProfile == Reaper)
                    activeProfile = Desktop;
            }
            break;

        case 3:
            if (!modRow)
            {
                switch (activeProfile)
                {
                case Games:
                    activeProfile = Desktop;
                    break;

                case SketchUp:
                    activeProfile = Graphic;
                    break;

                case KiCad:
                    activeProfile = Coding;
                    break;

                default:
                    break;
                }
            }
            else
            {
                if (activeProfile == Fusion)
                    activeProfile = Graphic;
            }
            break;

        case 4:
            if (!modRow)
            {
                if (activeProfile == Cinema4D)
                    activeProfile = Graphic;
            }
            else
            {
                switch (activeProfile)
                {
                case Graphic:
                    activeProfile = Desktop;
                    break;

                case Blender:
                    activeProfile = Graphic;
                    break;

                default:
                    break;
                }
            }
            break;

        case 5:
            if (!modRow)
            {
                if (activeProfile == Unity)
                    activeProfile = Graphic;
            }
            else
            {
                if (activeProfile == Unreal)
                    activeProfile = Graphic;
            }
            break;

        case 6:
            if (!modRow)
            {
                if (activeProfile == TouchDesigner)
                    activeProfile = Graphic;
            }
            else
            {
                /*code*/
            }
            break;

        default:
            break;
        }
    }
}

// STYLE hold and longHold colours are placeholders
/* Keypresses sent after release
 * This way short press macros won't activate before press length is determined
 */
void handleMatrix()
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
                    lastState[r][c] = idle;
                    break;

                case pressed:
                    lastState[r][c] = pressed;
                    break;

                case hold:
                    lastState[r][c] = hold;
                    led.setPixelColor(swLedId[r][c], grey);
                    break;

                case longHold:
                    lastState[r][c] = longHold;
                    led.setPixelColor(swLedId[r][c], white);
                    break;

                case released:
                    switch (lastState[r][c])
                    {
                    case pressed:
                        for (byte k = 0; k < MAX_MACRO; k++)
                        {
                            Keyboard.press(matrixBase[activeProfile][r][c][k]);
                        }
                        Keyboard.releaseAll();
                        break;

                    case hold:
                        for (byte k = 0; k < MAX_MACRO; k++)
                        {
                            Keyboard.press(matrixHold[activeProfile][r][c][k]);
                        }
                        Keyboard.releaseAll();
                        led.setPixelColor(swLedId[r][c], ledProfiles[activeProfile][r][c]);
                        break;

                    case longHold:
                        if (bitRead(swHold[r], c))
                        {
                            for (byte s = 0; s < 2; s++)
                            {
                                swHold[s] &= 0b00000000;
                            }
                        }
                        else
                        {
                            byte mask = 00000000;
                            swHold[r ? 0 : 1] &= mask;
                            bitSet(mask, c);
                            swHold[r] &= mask;
                        }
                        selectProfile(r, c);
                        updateLeds();
                        break;

                    default:
                        break;
                    }
                    lastState[r][c] = released;
                    break;

                default:
                    break;
                }
            }
        }
        led.show();
    }
}

// ?? can pass by ref be used here?
/* Keypresses sent after release
 * This way short press macros won't activate before press length is determined
 */
void handleSwitch(int pin, int id)
{
    if (handleState(sw.digitalRead(pin), id, 2))
    {
        switch (keyStates[2][id])
        {
        case idle:
            lastState[2][id] = idle;
            break;

        case pressed:
            lastState[2][id] = pressed;
            break;

        case hold:
            lastState[2][id] = hold;
            break;

        case released:
            switch (lastState[2][id])
            {
            case pressed:
                for (byte k = 0; k < MAX_MACRO; k++)
                {
                    Keyboard.press(swBase[activeProfile][bitRead(swHold[2], id)][id][k]);
                }
                Keyboard.releaseAll();
                break;

            case hold:
                // byte bitId = (id % 2) ? id : id + 4;
                bitWrite(swHold[2], id, !bitRead(swHold[2], id));
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

void handleSwitches(volatile bool &SW_awakenByInterrupt)
{
    if (SW_awakenByInterrupt)
    {
        switch (sw.getLastInterruptPin())
        {
        case R_SW_1:
            handleSwitch(R_SW_1, 0);
            break;

        case R_SW_2:
            handleSwitch(R_SW_2, 1);
            break;

        case R_SW_3:
            handleSwitch(R_SW_3, 2);
            break;

        case R_SW_4:
            handleSwitch(R_SW_4, 3);
            break;

        case SW_15:
            handleSwitch(SW_15, 4);
            break;

        case SW_16:
            handleSwitch(SW_16, 5);
            break;

        default:
            break;
        }

        SW_awakenByInterrupt = false;
    }
}