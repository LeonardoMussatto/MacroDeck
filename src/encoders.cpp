// ENHANCE to integrate position and rotation count see https://github.com/siktec-lab/SIK-Rotary-Encoder/blob/master/SIKTEC_Rotary.cpp

#include "encoders.h"

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
        handleEncoder(R1.read(), R1.speed(), 0, profile);
    if (re.getLastInterruptPin() == RE_2A || re.getLastInterruptPin() == RE_2B)
        handleEncoder(R2.read(), R2.speed(), 1, profile);
    if (re.getLastInterruptPin() == RE_3A || re.getLastInterruptPin() == RE_3B)
        handleEncoder(R3.read(), R3.speed(), 2, profile);
    if (re.getLastInterruptPin() == RE_4A || re.getLastInterruptPin() == RE_4B)
        handleEncoder(R4.read(), R4.speed(), 3, profile);

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