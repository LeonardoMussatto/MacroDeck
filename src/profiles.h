#ifndef PROFILES_H
#define PROFILES_H
#include <Keyboard.h>

#define MAX_PROFILES 7
#define MAX_MACRO 2

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

// REM SW_15/16 could be used as mod toggles - e.g. toggle hold
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
char macroBase[MAX_PROFILES][2][7][MAX_MACRO] =
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

char macroHold[MAX_PROFILES][2][7][MAX_MACRO] = {};

#endif