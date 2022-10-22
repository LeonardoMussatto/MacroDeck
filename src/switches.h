#ifndef SWITCHES_H
#define SWITCHES_H
#include "profiles.h"

void sw_begin();
void handleMatrix();
void handleSwitches(volatile bool &SW_awakenByInterrupt);

#endif