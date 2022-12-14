#ifndef PROFILES_H
#define PROFILES_H
#include <Keyboard.h>

#define MAX_PROFILES 7
#define ALT_PROFILES 1
#define MAX_MACRO 3
#define MAX_RE 4
#define DIR 2

enum Profiles
{
    Desktop,
    Firefox,
    Reaper,
    DaVinci_Fairlight,
    DaVinci_Colour,
    DaVinci_Edit,
    Graphic,
    Blender,
    Cinema4D,
    Unity,
    Unreal,
    SketchUp,
    Fusion,
    TouchDesigner,
    Coding,
    KiCad,
    File_Explorer,
    Games
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

extern const uint32_t profilesColours[MAX_PROFILES][2];
extern const unsigned char matrixBase[MAX_PROFILES][2][7][MAX_MACRO];
extern const unsigned char matrixHold[MAX_PROFILES][2][7][MAX_MACRO];
extern const unsigned char swBase[MAX_PROFILES][2][6][MAX_MACRO];
extern const unsigned char re_macroSlow[MAX_PROFILES][2][MAX_RE][DIR][MAX_MACRO];
extern const unsigned char re_macroFast[MAX_PROFILES][2][MAX_RE][DIR][MAX_MACRO];

String decodeProfile(Profiles profile);
void selectProfile(byte &modRow, byte &modCol);

#endif