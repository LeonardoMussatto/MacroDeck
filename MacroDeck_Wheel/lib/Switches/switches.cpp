#include "switches.h"
#include <Adafruit_MCP23X17.h>

// mcp 23017 over I2C
#define SW_ADDR 0x20
Adafruit_MCP23X17 sw;

// Independent switches
#define SW_15 5  // pin 26
#define SW_16 6  // pin 27
#define R_SW_1 4 // pin 25
#define R_SW_2 7 // pin 28
#define R_SW_3 3 // pin 24
#define R_SW_4 2 // pin 23

// Matrix
#define ROWS 2
#define COLS 7

// Keys
State lastState[3][7] = {idle};

// Timers
unsigned long lastTime;
const int debounceTime = 10;
const int shortPress = 500;
const int longPress = 1000;
unsigned long holdTimer;

volatile bool SW_awakenByInterrupt = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Init mcp, SW_matrix, RE_switches, T_switches & respective mcp interrupts
 */
void sw_begin()
{
    sw.begin_I2C(SW_ADDR);

    // init matrix
    const byte rowPins[ROWS] = {9, 10};                    // pin 2,3
    const byte colPins[COLS] = {11, 14, 15, 8, 12, 13, 0}; // pin 4,7,8,1,5,6,21

    for (byte r = 0; r < ROWS; r++)
    {
        sw.pinMode(rowPins[r], INPUT_PULLUP);
    }
    for (byte c = 0; c < COLS; c++)
    {
        sw.pinMode(colPins[c], OUTPUT);
    }

    // use SW_INT as interrupt pin
    pinMode(SW_INT, INPUT_PULLUP);
    PCICR |= B00000001;
    PCMSK0 |= B00010000;

    // init interrupt on change for R_SW_# and SW_15/16
    sw.setupInterrupts(false, false, LOW);

    const byte rSwitches[4] = {R_SW_1, R_SW_2, R_SW_3, R_SW_4};
    const byte tSwitches[2] = {SW_15, SW_16};

    for (byte s = 0; s < 4; s++)
    {
        sw.pinMode(rSwitches[s], INPUT_PULLUP);
        sw.setupInterruptPin(rSwitches[s], LOW);
    }
    for (byte s = 0; s < 2; s++)
    {
        sw.pinMode(tSwitches[s], INPUT_PULLUP);
        sw.setupInterruptPin(tSwitches[s], LOW);
    }
    sw.clearInterrupts();

    Keyboard.begin();

    lastTime = 0;
}

/**
 * @brief Pulse each column and store ALL keys in bitmap
 * @param bitMap[] bitMap array to store current pinStates
 */
void scanKeys(byte *bitMap)
{
    const byte rowPins[ROWS] = {9, 10};                    // pin 2,3
    const byte colPins[COLS] = {11, 14, 15, 8, 12, 13, 0}; // pin 4,7,8,1,5,6,21

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

/**
 * @brief Read last state and write new state
 *
 * @param closed HIGH/LOW - switch state
 * @param c key column in bitmap
 * @param r key row in bitmap
 * @param keyStates[][7] states array
 * @return true/false - whether the key state changed
 */
bool handleState(bool closed, const byte &c, const byte &r, State (*keyStates)[7])
{
    switch (lastState[r][c])
    {
    case idle:
        if (closed)
        {
            keyStates[r][c] = pressed;
            lastState[r][c] = keyStates[r][c];
            holdTimer = millis();
            return true;
        }
        break;

    case pressed:
        if ((millis() - holdTimer) > shortPress)
        {
            keyStates[r][c] = hold;
            lastState[r][c] = keyStates[r][c];
            return true;
        }
        else if (!closed)
        {
            keyStates[r][c] = released;
            lastState[r][c] = keyStates[r][c];
            return true;
        }
        break;

    case hold:
        if ((millis() - holdTimer) > longPress)
        {
            keyStates[r][c] = longHold;
            lastState[r][c] = keyStates[r][c];
            return true;
        }
        else if (!closed)
        {
            keyStates[r][c] = released;
            lastState[r][c] = keyStates[r][c];
            return true;
        }
        break;

    case longHold:
        if (!closed)
        {
            keyStates[r][c] = released;
            lastState[r][c] = keyStates[r][c];
            return true;
        }
        break;

    case released:
        keyStates[r][c] = idle;
        break;

    default:
        break;
    }
    return false;
}

/**
 * @brief calls handleState() for each key in the matrix
 *
 * @param keyStates[][7] states array
 * @return true/false - whether any state change occurred
 */
bool updateStates(byte *bitMap, State (*keyStates)[7])
{
    bool anyActivity = false;

    for (byte c = 0; c < COLS; c++)
    {
        for (byte r = 0; r < ROWS; r++)
        {
            bool activity = handleState(bitRead(bitMap[r], c), c, r, keyStates);
            if (!anyActivity && activity)
                anyActivity = true;
        }
    }

    return anyActivity;
}

/**
 * @brief Scan matrix if enough time passed since last scan (debounce), then call updateStates()
 *
 * @param KeyStates[][7] states array
 * @return true/false - whether at leas one state change occurred
 */
bool getKeys(State (*keyStates)[7])
{
    /* Switches' bitMap layout - 7 -> 0
     * SW13 | SW11 | SW9  | SW7 | SW5 | SW3 | SW1
     * SW14 | SW12 | SW10 | SW8 | SW6 | SW4 | SW2
     */
    byte bitMap[2]{};
    bool keyActivity = false;

    // Limit how often the matrix is scanned. It makes the loop() faster
    if ((millis() - lastTime) > debounceTime)
    {
        scanKeys(bitMap);
        keyActivity = updateStates(bitMap, keyStates);
        lastTime = millis();
    }

    return keyActivity;
}

/**
 * @brief Call getKeys() and handle keys if any change occurred.
 * @note Keypresses are sent after release. This way press macros won't activate before press length is determined
 */
void handleMatrix()
{
    State keyStates[2][7] = {idle};
    if (getKeys(keyStates))
    {
        for (byte c = 0; c < COLS; c++)
        {
            for (byte r = 0; r < ROWS; r++)
            {
                if (keyStates[r][c] == released)
                {
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
                            byte mask = 0b00000000;
                            swHold[r ? 0 : 1] &= mask;
                            bitSet(mask, c);
                            swHold[r] = mask;
                        }
                        selectProfile(r, c);
                        break;

                    default:
                        break;
                    }
                    lastState[r][c] = released;
                }
            }
        }
    }
}
// ?? can pass by ref be used here?
// ?? sw.getCapturedInterrupt() might be more appropriate, but I'm not sure how to select the correct pin
/**
 * @brief Call handleState() for the specified key and handle state change
 *
 * @param pin pin that caused the interrupt
 * @param id column in state maps
 */
void handleSwitch(int pin, int id)
{
    State swState[1][7] = {idle};
    if (handleState(!sw.digitalRead(pin), id, 2, swState))
    {
        switch (swState[2][id])
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

/**
 * @brief If an interrupt occurred, call handleSwitch() for the key that caused the interrupt
 */
void handleSwitches()
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
    sw.clearInterrupts();
}