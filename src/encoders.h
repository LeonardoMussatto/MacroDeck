#ifndef ENCODERS_H
#define ENCODERS_H
#include "profiles.h"

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

#define FAST 100
MD_REncoder R1 = MD_REncoder(RE_1A, RE_1B);
MD_REncoder R2 = MD_REncoder(RE_2A, RE_2B);
MD_REncoder R3 = MD_REncoder(RE_3A, RE_3B);
MD_REncoder R4 = MD_REncoder(RE_4A, RE_4B);

volatile bool RE_awakenByInterrupt;

void re_begin();
void handleEncoders(Profiles &profile);

#endif