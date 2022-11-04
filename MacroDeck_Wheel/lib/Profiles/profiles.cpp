#include "profiles.h"

// REM potentially each switch could circle between many states
// REM SW_15/16 could be used as mod toggles - e.g. toggle hold
// TODO add settings shortcut to desktop
// TODO add R_SW

/*  [n of profiles][rows][cols][keys]
 *                   |     |
 *                as in keyStates
 *-----------------------------------------------
 *    SW1  |   SW3  |   SW5  |   SW7  |  SW9  |  SW11  |  SW13
 *    SW2  |   SW4  |   SW6  |   SW8  |  SW10 |  SW12  |  SW14
 *  R_SW_1 | R_SW_2 | R_SW_3 | R_SW_4 | SW_15 | SW_16  |  --
 */

// {// prototype
//  {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}},
//  {{/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}, {/**/}}},

State keyStates[3][7] = {idle};
State lastState[3][7] = {idle};
byte swHold[3] = {00000000};

Profiles activeProfile = Desktop;

String decodeProfile(Profiles profile)
{
    switch (profile)
    {
    case Firefox:
        return "Firefox";
    case Reaper:
        return "Reaper";
    case DaVinci_Fairlight:
        return "DaVinci - Fairlight";
    case DaVinci_Colour:
        return "DaVinci - Colour";
    case DaVinci_Edit:
        return "DaVinci - Edit";
    case Unity:
        return "Unity";
    case Unreal:
        return "Unreal";
    case Blender:
        return "Blender";
    case Fusion:
        return "Fusion 360";
    case SketchUp:
        return "SketchUp";
    case File_Explorer:
        return "File Explorer";
    case Games:
        return "Games";
    case Graphic:
        return "Graphic";
    case Coding:
        return "Coding";
    case Desktop:
        return "Desktop";
    default:
        return "Error";
    }
}

uint32_t profilesColours[MAX_PROFILES][2] =
    {
        {0x7FF /*Cyan*/, 0x7E0 /*Green*/}};

unsigned char matrixBase[MAX_PROFILES][2][7][MAX_MACRO] =
    {
        {// Desktop
         {{KEY_LEFT_GUI, '2', 0 /*Control Center*/}, {KEY_LEFT_GUI, '7', 0 /*DaVinci*/}, {KEY_LEFT_GUI, '3', 0 /*Firefox*/}, {0, 0, 0 /*Games*/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'm' /*Spotify*/}},
         {{KEY_LEFT_GUI, 'e', 0 /*File Explorer*/}, {KEY_LEFT_GUI, '4', 0 /*VsCode - Coding*/}, {KEY_LEFT_GUI, '8', 0 /*Reaper*/}, {0, 0, 0 /**/}, {0, 0, 0 /*3D*/}, {0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'w' /*Whatsapp*/}}},
        {// Firefox
         {{0, 0, 0 /**/}, {KEY_LEFT_CTRL, 'h', 0 /*History*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'w' /*Stash Tab*/}, {0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'k' /*Console*/}, {KEY_LEFT_CTRL, 'd', 0 /*Bookmark Tab*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 's' /*Stashed Tab Sidebar*/}},
         {{0, 0, 0 /**/}, {KEY_LEFT_CTRL, 'b', 0 /*Bookmarks*/}, {KEY_LEFT_CTRL, 'n', 0 /*New Window*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'M' /*Responsive*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'c' /*Inspector*/}, {KEY_LEFT_ALT, KEY_HOME, 0 /*Home*/}, {KEY_LEFT_CTRL, 'q', 0 /*Quit*/}}},
        {// Graphic
         {{0, 0, 0 /**/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'd' /*SketchUp*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'c' /*Cinema 4D*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'u' /*Unity Hub*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 't' /*TouchDesigner*/}},
         {{0, 0, 0 /**/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'f' /*Fusion*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'b' /*Blender*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'e' /*Epic*/}, {0, 0, 0 /**/}}},
        {// Coding
         {{KEY_LEFT_GUI, '5', 0 /*GitKraken*/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'k' /*KiCad*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'a' /*Arduino*/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}},
         {{0, 0, 0 /**/}, {KEY_LEFT_GUI, '4', 0 /*VsCode*/}, {KEY_LEFT_CTRL, 39, 0 /*Terminal*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'p' /*Processing*/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}, {0, 0, 0 /**/}}},
        {// File Explorer
         {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '1' /*Documents*/}, {KEY_LEFT_ALT, KEY_RETURN, 0 /*Properties*/}, {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'n' /*New Folder*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '3' /*Music*/}, {/*Current Project*/}, {KEY_LEFT_CTRL, 'v', 0 /*Paste*/}, {KEY_LEFT_CTRL, 'c', 0 /*Copy*/}},
         {{KEY_LEFT_CTRL, KEY_LEFT_ALT, '0' /*Projects*/}, {KEY_LEFT_ALT, 'p', 0 /*Preview*/}, {KEY_LEFT_CTRL, 'n', 0 /*New Window*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, '2' /*Downloads*/}, {KEY_F2, 0, 0 /*Rename*/}, {KEY_LEFT_CTRL, 'x', 0 /*Cut*/}, {KEY_LEFT_CTRL, 'w', 0 /*Close*/}}},
        {// Games
         {{KEY_LEFT_GUI, '2', 0 /*Control Center*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'f' /*The Forest*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'g' /*Genshin*/}, {/**/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'e' /*Epic*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'l' /*Paladins*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'm' /*Surviving Mars*/}},
         {{0, 0, 0 /**/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 't' /*Titafall*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'h' /*Horizon*/}, {/**/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 's' /*Steam*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'c' /*Civilization*/}, {KEY_LEFT_CTRL, KEY_LEFT_ALT, 'n' /*NMS*/}}},
};

unsigned char matrixHold[MAX_PROFILES][2][7][MAX_MACRO] = {};

unsigned char swBase[MAX_PROFILES][2][6][MAX_MACRO] =
    {
        {// Desktop
         {{/**/}, {/**/}, {KEY_RIGHT_ALT, KEY_F1, 0 /*Screen Off*/}, {KEY_RIGHT_ALT, KEY_F10, 0 /*Play/Pause*/}, {/**/}, {/**/}},
         {{/**/}, {/**/}, {KEY_RIGHT_ALT, KEY_F6, 0 /*Lock*/}, {KEY_RIGHT_ALT, KEY_F7, 0 /*Mute*/}, {/**/}, {/**/}}},
};

unsigned char re_macroSlow[MAX_PROFILES][2][MAX_RE][DIR][MAX_MACRO] = {
    {// Desktop
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F4}, {KEY_RIGHT_ALT, KEY_F3} /*Brightness +/- */}, {{KEY_RIGHT_ALT, KEY_F9}, {KEY_RIGHT_ALT, KEY_F8} /*Volume +/- */}},
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F1}, {KEY_RIGHT_ALT, KEY_F6} /*Sleep/Lock*/}, {{KEY_RIGHT_ALT, KEY_F7}, {KEY_RIGHT_ALT, KEY_F12} /*Mute/Circle Sound Devices*/}}},
};

unsigned char re_macroFast[MAX_PROFILES][2][MAX_RE][DIR][MAX_MACRO] = {
    {// Desktop
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F4}, {KEY_RIGHT_ALT, KEY_F3} /*Brightness +/- */}, {{KEY_RIGHT_ALT, KEY_F9}, {KEY_RIGHT_ALT, KEY_F8} /*Volume +/- */}},
     {{/**/}, {/**/}, {{KEY_RIGHT_ALT, KEY_F1}, {KEY_RIGHT_ALT, KEY_F6} /*Sleep/Lock*/}, {{KEY_RIGHT_ALT, KEY_F7}, {KEY_RIGHT_ALT, KEY_F12} /*Mute/Circle Sound Devices*/}}},
};