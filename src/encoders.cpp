// ENHANCE to integrate position and rotation count see https://github.com/siktec-lab/SIK-Rotary-Encoder/blob/master/SIKTEC_Rotary.cpp

#include "encoders.h"
#include <Adafruit_MCP23X08.h>
#include <MD_REncoder.h>

#define RE_INT 7
#define RE_ADDR 0x21
Adafruit_MCP23X08 mcp;

#define RE_1A 11
#define RE_1B 10
#define RE_2A 13
#define RE_2B 12
#define RE_3A 16
#define RE_3B 17
#define RE_4A 14
#define RE_4B 15
const byte rotaryEncPins[2][4] = {{RE_1A, RE_2A, RE_3A, RE_4A}, {RE_1B, RE_2B, RE_3B, RE_4B}};
R1 = MD_REncoder(RE_1A, RE_1B);
R2 = MD_REncoder(RE_2A, RE_2B);
R3 = MD_REncoder(RE_3A, RE_3B);
R4 = MD_REncoder(RE_4A, RE_4B);

volatile bool RE_awakenByInterrupt;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void re_begin()
{
    mcp.begin_I2C(RE_ADDR);
    mcp.setupInterrupts(false, false, LOW);

    attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);

    // init pins as inputs and interrupts on change
    for (e = 0; e < 4; e++)
    {
        for (p = 0; p < 2; p++)
        {
            mcp.pinMode(rotaryEncPins[p][e], INPUT);
            mcp.setupInterruptPin(rotaryEncPins[p][e], CHANGE);
        }
    }

    R1.begin();
    R2.begin();
    R3.begin();
    R4.begin();
}

void RE_intCallBack()
{
    RE_awakenByInterrupt = true;
}

void handleEncoder()
{
    if (mcp.getLastInterruptPin == RE_1A || mcp.getLastInterruptPin == RE_1B)
    {
        R1.read() == DIR_CW ?
        {
            R1.speed() > SLOW ? /*something fast*/: /*something slow*/;
        }
        :
        {
            R1.speed() > SLOW ? /*something else fast*/: /*something else*/
        }
    }
    if (mcp.getLastInterruptPin == RE_2A || mcp.getLastInterruptPin == RE_2B)
    {
        R2.read() == DIR_CW ?
        {
            R2.speed() > SLOW ? /*something fast*/: /*something slow*/;
        }
        :
        {
            R2.speed() > SLOW ? /*something else fast*/: /*something else*/
        }
    }
    if (mcp.getLastInterruptPin == RE_3A || mcp.getLastInterruptPin == RE_3B)
    {
        R3.read() == DIR_CW ?
        {
            R3.speed() > SLOW ? /*something fast*/: /*something slow*/;
        }
        :
        {
            R3.speed() > SLOW ? /*something else fast*/: /*something else*/
        }
    }
    if (mcp.getLastInterruptPin == RE_4A || mcp.getLastInterruptPin == RE_4B)
    {
        R4.read() == DIR_CW ?
        {
            R4.speed() > SLOW ? /*something fast*/: /*something slow*/;
        }
        :
        {
            R4.speed() > SLOW ? /*something else fast*/: /*something else*/
        }
    }

    SW_awakenByInterrupt = false;
}

void checkEncoders()
{
    if (awakenByInterrupt)
    {
        detachInterrupt(digitalPinToInterrupt(RE_INT));
        handleEncoder();
        attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack(), FALLING);
    }
}