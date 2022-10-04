// ENHANCE to integrate position and rotation count see https://github.com/siktec-lab/SIK-Rotary-Encoder/blob/master/SIKTEC_Rotary.cpp

#include "encoders.h"
#include <Adafruit_MCP23X08.h>
#include <MD_REncoder.h>

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
const byte rotaryEncPins[2][4] = {{RE_1A, RE_2A, RE_3A, RE_4A}, {RE_1B, RE_2B, RE_3B, RE_4B}};

#define SLOW 100
MD_REncoder R1 = MD_REncoder(RE_1A, RE_1B);
MD_REncoder R2 = MD_REncoder(RE_2A, RE_2B);
MD_REncoder R3 = MD_REncoder(RE_3A, RE_3B);
MD_REncoder R4 = MD_REncoder(RE_4A, RE_4B);

volatile bool RE_awakenByInterrupt;

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void re_begin()
{
    re.begin_I2C(RE_ADDR);
    re.setupInterrupts(false, false, LOW);

    attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);

    // init pins as inputs and interrupts on change
    for (byte e = 0; e < 4; e++)
    {
        for (byte p = 0; p < 2; p++)
        {
            re.pinMode(rotaryEncPins[p][e], INPUT);
            re.setupInterruptPin(rotaryEncPins[p][e], CHANGE);
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

void findEncoder()
{
    if (re.getLastInterruptPin() == RE_1A || re.getLastInterruptPin() == RE_1B)
        handleEncoder(R1.read(), R1.speed(), 1);
    if (re.getLastInterruptPin() == RE_2A || re.getLastInterruptPin() == RE_2B)
        handleEncoder(R2.read(), R2.speed(), 2);
    if (re.getLastInterruptPin() == RE_3A || re.getLastInterruptPin() == RE_3B)
        handleEncoder(R3.read(), R3.speed(), 3);
    if (re.getLastInterruptPin() == RE_4A || re.getLastInterruptPin() == RE_4B)
        handleEncoder(R4.read(), R4.speed(), 4);

    RE_awakenByInterrupt = false;
}

void handleEncoder(byte dir, uint16_t spd, byte id)
{
    if (dir == DIR_CW)
    {
        if (spd > SLOW)
        { /*something fast*/
        }
        else
        { /*something slow*/
        }
    }
    else
    {
        if (spd > SLOW)
        { /*something else fast*/
        }
        else
        { /*something else slow*/
        }
    }
}

void checkEncoders()
{
    if (RE_awakenByInterrupt)
    {
        detachInterrupt(digitalPinToInterrupt(RE_INT));
        findEncoder();
        attachInterrupt(digitalPinToInterrupt(RE_INT), RE_intCallBack, FALLING);
    }
}