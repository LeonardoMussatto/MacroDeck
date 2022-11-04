#ifndef SWITCHES_H
#define SWITCHES_H
#include <profiles.h>

extern volatile bool SW_awakenByInterrupt;

void sw_begin();
void handleMatrix();
void handleSwitches();

#endif