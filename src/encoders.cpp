// ENHANCE to integrate position and rotation count see https://github.com/siktec-lab/SIK-Rotary-Encoder/blob/master/SIKTEC_Rotary.cpp

#include "encoders.h"

#include <Adafruit_MCP23X08.h>
#include <MD_REncoder_OverMCP.h>

#define RE_INT 7
#define RE_ADDR 0x21
Adafruit_MCP23X08 re;

#define RE_1A 11
#define RE_1B 10
#define RE_2A 13
#define RE_2B 12
#define RE_3A 16
#define RE_3B 17
#define RE_4A 14
#define RE_4B 15

#define FAST 100
MD_REncoder_MCP R1 = MD_REncoder_MCP(RE_1A, RE_1B);
MD_REncoder_MCP R2 = MD_REncoder_MCP(RE_2A, RE_2B);
MD_REncoder_MCP R3 = MD_REncoder_MCP(RE_3A, RE_3B);
MD_REncoder_MCP R4 = MD_REncoder_MCP(RE_4A, RE_4B);

volatile bool RE_awakenByInterrupt;

///////////////////////////////////////////////////////////////////////////////////////////////////

void re_begin()
{
    re.begin_I2C(RE_ADDR);
    re.setupInterrupts(false, false, LOW);

    attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);

    // init pins as inputs and interrupts on change
    R1.begin(re);
    R2.begin(re);
    R3.begin(re);
    R4.begin(re);
}

void RE_intCallBack()
{
    RE_awakenByInterrupt = true;
}

void handleEncoder(byte dir, uint16_t spd, byte id, Profiles &profile)
{
    if (spd < FAST)
    {
        for (byte k = 0; k < MAX_MACRO; k++)
        {
            Keyboard.press(re_macroSlow[profile][keyStates[3][id] == 0 ? 1 : 2][id][dir][k]);
        }
    }
    else
    {
        for (byte k = 0; k < MAX_MACRO; k++)
        {
            Keyboard.press(re_macroFast[profile][keyStates[3][id] == 0 ? 1 : 2][id][dir][k]);
        }
    }
}

void selectEncoder(Profiles &profile)
{
    if (re.getLastInterruptPin() == RE_1A || re.getLastInterruptPin() == RE_1B)
        handleEncoder(R1.read(re), R1.speed(), 0, profile);
    if (re.getLastInterruptPin() == RE_2A || re.getLastInterruptPin() == RE_2B)
        handleEncoder(R2.read(re), R2.speed(), 1, profile);
    if (re.getLastInterruptPin() == RE_3A || re.getLastInterruptPin() == RE_3B)
        handleEncoder(R3.read(re), R3.speed(), 2, profile);
    if (re.getLastInterruptPin() == RE_4A || re.getLastInterruptPin() == RE_4B)
        handleEncoder(R4.read(re), R4.speed(), 3, profile);

    RE_awakenByInterrupt = false;
}

void handleEncoders(Profiles &profile)
{
    if (RE_awakenByInterrupt)
    {
        detachInterrupt(digitalPinToInterrupt(RE_INT));
        selectEncoder(profile);
        attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);
    }
}