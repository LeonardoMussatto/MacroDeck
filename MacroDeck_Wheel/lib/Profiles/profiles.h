#ifndef PROFILES_H
#define PROFILES_H
#include <Keyboard.h>

const byte MAX_PROFILES = 15;
// const byte ALT_PROFILES = 1;

enum Profiles
{
    Desktop,
    Graphic,
    Coding,
    Firefox,
    Reaper,
    DaVinci_Fairlight,
    DaVinci_Colour,
    DaVinci_Edit,
    PremierePro,
    Blender,
    SketchUp,
    TouchDesigner,
    File_Explorer,
};
extern Profiles activeProfile;

enum State
{
    idle,
    pressed,
    hold,
    longHold,
    released
};

extern byte swHold[3];

// extern const uint32_t profilesColours[MAX_PROFILES][2];

String decodeProfile(Profiles profile);

#endif