#include <encoders.h>
#include <switches.h>

void setup()
{
    sw_begin();
    re_begin();

    activeProfile = Desktop;
}

void loop()
{
    handleMatrix();
    handleEncoders();
    handleSwitches();
}

ISR(PCINT0_vect)
{
    if (!digitalRead(SW_INT))
        SW_awakenByInterrupt = true;
}