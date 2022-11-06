// ENHANCE to integrate position and rotation count see https://github.com/siktec-lab/SIK-Rotary-Encoder/blob/master/SIKTEC_Rotary.cpp

#include "encoders.h"

#include <Adafruit_MCP23X08.h>
#include <MD_REncoder_OverMCP.h>

#define RE_INT 7
#define RE_ADDR 0x21
Adafruit_MCP23X08 re;

#define RE_1A 1
#define RE_1B 0
#define RE_2A 3
#define RE_2B 2
#define RE_3A 6
#define RE_3B 7
#define RE_4A 4
#define RE_4B 5

#define FAST 100
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
    if (spd < FAST)
    {
        for (byte k = 0; k < MAX_MACRO; k++)
        {
            Keyboard.press(re_macroSlow[activeProfile][bitRead(swHold[2], id)][id][dir][k]);
        }
    }
    else
    {
        for (byte k = 0; k < MAX_MACRO; k++)
        {
            Keyboard.press(re_macroFast[activeProfile][bitRead(swHold[2], id)][id][dir][k]);
        }
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
    {
        detachInterrupt(digitalPinToInterrupt(RE_INT));
        selectEncoder();
    }

    /* Not optimal position, but it seems the only way to make it work
     * Before reattaching in order to avoid clearing a new interrupt
     */
    re.clearInterrupts();

    if (RE_awakenByInterrupt)
        attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);
}