// ENHANCE to integrate position and rotation count see https://github.com/siktec-lab/SIK-Rotary-Encoder/blob/master/SIKTEC_Rotary.cpp

#include "encoders.h"

#include <Adafruit_MCP23X08.h>
#include <MD_REncoder_OverMCP.h>

const byte RE_INT = 7;
const byte MAX_RE = 4;
const byte DIR = 2;

#define RE_ADDR 0x21
Adafruit_MCP23X08 re;

const byte RE_1A = 1;
const byte RE_1B = 0;
const byte RE_2A = 3;
const byte RE_2B = 2;
const byte RE_3A = 6;
const byte RE_3B = 7;
const byte RE_4A = 4;
const byte RE_4B = 5;

const byte FAST = 100;

MD_REncoder_MCP R1 = MD_REncoder_MCP(RE_1A, RE_1B);
MD_REncoder_MCP R2 = MD_REncoder_MCP(RE_2A, RE_2B);
MD_REncoder_MCP R3 = MD_REncoder_MCP(RE_3A, RE_3B);
MD_REncoder_MCP R4 = MD_REncoder_MCP(RE_4A, RE_4B);

volatile bool RE_awakenByInterrupt = false;

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Set RE_awakenByInterrupt to TRUE
 *
 */
void RE_intCallBack()
{
    RE_awakenByInterrupt = true;
}

/**
 * @brief Init mcp, interrupts and encoders
 *
 */
void re_begin()
{
    // MCP init
    re.begin_I2C(RE_ADDR);
    re.setupInterrupts(false, false, LOW);

    // arduino interrupt setup
    pinMode(RE_INT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);

    // init MCP pins as inputs and interrupts on change
    R1.begin(re);
    R2.begin(re);
    R3.begin(re);
    R4.begin(re);

    re.clearInterrupts();
}

/**
 * @brief Check encoder speed and send the corresponding keypresses as defined in Profiles
 *
 * @param dir rotation direction
 * @param spd speed
 * @param id encoder index in keyStates
 */
void handleEncoder(byte dir, uint16_t spd, byte id)
{
    const unsigned char re_macroSlow[2][MAX_RE][DIR][2] = {
     {{{KEY_RIGHT_ALT, KEY_F15}, {KEY_LEFT_ALT, KEY_F15}}, {{KEY_RIGHT_ALT, KEY_F16}, {KEY_LEFT_ALT, KEY_F16}}, {{KEY_RIGHT_ALT, KEY_F17}, {KEY_LEFT_ALT, KEY_F17}}, {{KEY_RIGHT_ALT, KEY_F18}, {KEY_LEFT_ALT, KEY_F18}}},
     {{{KEY_RIGHT_ALT, KEY_F19}, {KEY_LEFT_ALT, KEY_F19}}, {{KEY_RIGHT_ALT, KEY_F20}, {KEY_LEFT_ALT, KEY_F20}}, {{KEY_RIGHT_ALT, KEY_F21}, {KEY_LEFT_ALT, KEY_F21}}, {{KEY_RIGHT_ALT, KEY_F22}, {KEY_LEFT_ALT, KEY_F22}}}
    };
    const unsigned char re_macroFast[2][MAX_RE][DIR][2] = {
     {{{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F15}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F15}}, {{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F16}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F16}}, {{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F17}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F17}}, {{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F18}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F18}}},
     {{{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F19}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F19}}, {{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F20}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F20}}, {{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F21}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F21}}, {{KEY_LEFT_SHIFT, KEY_RIGHT_ALT, KEY_F22}, {KEY_LEFT_SHIFT, KEY_LEFT_ALT, KEY_F22}}}
    };

    if (spd < FAST)
    {
        for (byte k = 0; k < 2; k++)
        {
            Keyboard.press(re_macroSlow[bitRead(swHold[2], id)][id][dir][k]);
        }
        Keyboard.releaseAll();
    }
    else
    {
        for (byte k = 0; k < 3; k++)
        {
            Keyboard.press(re_macroFast[bitRead(swHold[2], id)][id][dir][k]);
        }
        Keyboard.releaseAll();
    }
}

/**
 * @brief Get the pin that caused the interrupt and call handleEncoder() using the appropriate encoder object
 *
 */
void selectEncoder()
{
    if (re.getLastInterruptPin() == RE_1A || re.getLastInterruptPin() == RE_1B)
        handleEncoder(R1.read(re), R1.speed(), 0);
    else if (re.getLastInterruptPin() == RE_2A || re.getLastInterruptPin() == RE_2B)
        handleEncoder(R2.read(re), R2.speed(), 1);
    else if (re.getLastInterruptPin() == RE_3A || re.getLastInterruptPin() == RE_3B)
        handleEncoder(R3.read(re), R3.speed(), 2);
    else if (re.getLastInterruptPin() == RE_4A || re.getLastInterruptPin() == RE_4B)
        handleEncoder(R4.read(re), R4.speed(), 3);

    RE_awakenByInterrupt = false;
}

/**
 * @brief If an interrupt occurred, handle the encoder then clear interrupts
 *
 */
void handleEncoders()
{
    if (RE_awakenByInterrupt)
        selectEncoder();
    re.clearInterrupts();
}