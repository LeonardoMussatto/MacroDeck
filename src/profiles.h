#ifndef PROFILES_H
#define PROFILES_H
#include <Keyboard.h>

#define MAX_PROFILES 7
#define MAX_MACRO 2
#define MAX_RE 4
#define DIR 2

enum Profiles
{
    Desktop,
    Reaper,
    DaVinci_Fairlight,
    DaVinci_Colour,
    DaVinci_Edit,
    Blender,
    VsCode
};

enum State
{
    idle,
    pressed,
    hold,
    released
};
State keyStates[3][7] = {idle};

// REM SW_15/16 could be used as mod toggles - e.g. toggle hold
// ALERT undefined shortcut handling MISSING
// TODO add settings shortcut to desktop
// TODO add R_SW

/*  [n of profiles][rows][cols][keys]
 *                   |     |
 *                as in keyStates
 *-----------------------------------------------
 *  Switches' bitMap layout - 7 -> 0 - INVERT HERE
 *     SW13  |  SW11  |   SW9  |   SW7   |  SW5  |  SW3  |  SW1
 *    SW14  |  SW12  |  SW10  |   SW8   |  SW6  |  SW4  |  SW2
 *  R_SW_1 | R_SW_2 | R_SW_3 | R_SW_4 | SW_15 | SW_16  |  --
 *
 */
char sw_macroBase[MAX_PROFILES][2][7][MAX_MACRO] =
    {
        {// Desktop
         {{KEY_LEFT_GUI, '2' /*Control Center*/}, {KEY_LEFT_GUI, '7' /*DaVinci*/}, {KEY_LEFT_GUI, '3' /*Firefox*/}, {/*Horizon*/}, {/*NMS*/}, {KEY_LEFT_GUI, '5' /*GitKraken*/}, {/*Spotify*/}},
         {{KEY_LEFT_GUI, '1' /*Files*/}, {KEY_LEFT_GUI, '4' /*VsCode*/}, {KEY_LEFT_GUI, '8' /*Reaper*/}, {/*Genshin*/}, {KEY_LEFT_GUI, '6' /*Blender*/}, {KEY_LEFT_GUI, '9' /*KiCad*/}, {/*Whatsapp*/}}},
        {// Reaper
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},
        {// DaVinci_Fairlight
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},
        {// DaVinci_Colour
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},
        {// DaVinci_Edit
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},
        {// Blender
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},
        {// VsCode
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},
};

char sw_macroHold[MAX_PROFILES][2][7][MAX_MACRO] = {};

char re_macroSlow[MAX_PROFILES][2][MAX_RE][DIR][MAX_MACRO] = {
    {// Desktop
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F4}, {KEY_RIGHT_ALT, KEY_F3} /*Brightness +/- */}, {{KEY_RIGHT_ALT, KEY_F9}, {KEY_RIGHT_ALT, KEY_F8} /*Volume +/- */}},
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F1}, {KEY_RIGHT_ALT, KEY_F6} /*Sleep/Lock*/}, {{KEY_RIGHT_ALT, KEY_F7}, {KEY_RIGHT_ALT, KEY_F12} /*Mute/Circle Sound Devices*/}}},
    {// Reaper
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// DaVinci_Fairlight
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// DaVinci_Colour
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// DaVinci_Edit
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// Blender
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// VsCode
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
};

char re_macroFast[MAX_PROFILES][2][MAX_RE][DIR][MAX_MACRO] = {
    {// Desktop
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F4}, {KEY_RIGHT_ALT, KEY_F3} /*Brightness +/- */}, {{KEY_RIGHT_ALT, KEY_F9}, {KEY_RIGHT_ALT, KEY_F8} /*Volume +/- */}},
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F1}, {KEY_RIGHT_ALT, KEY_F6} /*Sleep/Lock*/}, {{KEY_RIGHT_ALT, KEY_F7}, {KEY_RIGHT_ALT, KEY_F12} /*Mute/Circle Sound Devices*/}}},
    {// Reaper
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// DaVinci_Fairlight
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// DaVinci_Colour
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// DaVinci_Edit
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// Blender
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
    {// VsCode
     {{/**/}, {/**/}, {/**/}, {/**/}},
     {{/**/}, {/**/}, {/**/}, {/**/}}},
};

#endif