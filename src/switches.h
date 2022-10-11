#ifndef SWITCHES_H
#define SWITCHES_H

void sw_begin();
void handleMatrix();
void handleSwitches(volatile bool &SW_awakenByInterrupt);

#endif