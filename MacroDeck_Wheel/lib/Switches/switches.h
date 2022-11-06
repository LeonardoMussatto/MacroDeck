#ifndef SWITCHES_H
#define SWITCHES_H
#include <profiles.h>

#define SW_INT 8 // Arduino PCint pin
extern volatile bool SW_awakenByInterrupt;

void sw_begin();
void handleMatrix();
void handleSwitches();

#endif