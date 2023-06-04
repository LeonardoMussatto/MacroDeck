#include "profiles.h"

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

byte swHold[3] = {00000000};

/**
 * @brief Return profile name as a String
 *
 * @param profile
 * @return String - profile name
 */
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
    case PremierePro:
        return "Premiere Pro";
    case TouchDesigner:
        return "TouchDesigner";
    // case Unreal:
    //     return "Unreal";
    case Blender:
        return "Blender";
    // case Fusion:
    //     return "Fusion 360";
    case SketchUp:
        return "SketchUp";
    case File_Explorer:
        return "File Explorer";
    // case Games:
    //     return "Games";
    case Graphic:
        return "Graphic";
    case Coding:
        return "Coding";
    case Desktop:
        return "Desktop";
    default:
        return "Unknown Profile";
    }
}

// const uint32_t profilesColours[MAX_PROFILES][2] =
//     {
//         {0x7FF /*Cyan*/, 0x7E0 /*Green*/}};