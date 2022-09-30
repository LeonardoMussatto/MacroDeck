#include "profiles.h"

void sw_begin();
void handleMatrix(Profiles &profile);
void handleSwitches(Profiles &profile, volatile bool &SW_awakenByInterrupt);