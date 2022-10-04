#ifndef SWITCHES_H
#define SWITCHES_H
#include "profiles.h"

void sw_begin();
void handleMatrix(Profiles &profile);
void handleSwitches(Profiles &profile, volatile bool &SW_awakenByInterrupt);

#endif