; #Requires AutoHotkey v2.0
SendMode "Input"  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir A_ScriptDir ; Ensures a consistent starting directory.
A_MenuMaskKey := "vkFF" ; vkFF is no mapping.

global Desktop := 0
global Graphic := 1
global Coding := 2
global Explorer := 3 
;global IsGames := 4
global ActivePage := Desktop
global PassThrough := False
global OnTop := False

;TODO Profiles
    ;Desktop,
    ;Graphic,
    ;Coding,
    ;Firefox,
    ;Reaper,
    ;DaVinci_Fairlight,
    ;DaVinci_Colour,
    ;DaVinci_Edit,
    ;PremierePro
    ;Blender,
    ;Unreal,
    ;SketchUp,
    ;Fusion,
    ;TouchDesigner,
    ;File_Explorer,
;Profiles
    GroupAdd "Profiles", "ahk_exe Code.exe"
    GroupAdd "Profiles", "ahk_exe Arduino IDE.exe"
    GroupAdd "Profiles", "ahk_exe firefox.exe"
    GroupAdd "Profiles", "ahk_exe reaper.exe"
    GroupAdd "Profiles", "ahk_exe Resolve.exe"
    GroupAdd "Profiles", "ahk_exe Adobe Premiere Pro.exe"
    GroupAdd "Profiles", "ahk_exe TouchDesigner.exe"
    GroupAdd "Profiles", "ahk_exe explorer.exe"
;

GroupAdd "Coding", "ahk_exe Code.exe"
GroupAdd "Coding", "ahk_exe Arduino IDE.exe"

CheckWindow()
{
    global
    if OnTop {
        if PassThrough
            ActivePage := 100
        else
            return
    } else if WinActive("ahk_group Profiles") {
        if WinActive("ahk_group Coding")
            ActivePage := Coding
        else if WinActive("ahk_exe explorer.exe")
            ActivePage := Explorer
        else
            ActivePage := 100
    } else {
        ActivePage := Desktop
    }
}
SetTimer CheckWindow, 500

ToggleWindow(WinTitle, PathToExe)
{
    if WinExist(WinTitle)
    {
        if WinActive()
            WinMinimize
        else
            WinActivate
    } else
        Run PathToExe
}

;Base
    +F23:: {
            global
            switch ActivePage{
                case Desktop:
                    MsgBox "Active Page: Desktop `n`OnTop: " OnTop
                case Graphic:
                    MsgBox "Active Page: Graphic `n`OnTop: " OnTop
                case Coding:
                    MsgBox "Active Page: Coding `n`OnTop: " OnTop
                case Explorer:
                    MsgBox "Active Page: Explorer `n`OnTop: " OnTop
                default:
                    MsgBox "Active Page: Other `n`OnTop: " OnTop
            }
        }
    ^F23:: global OnTop := !OnTop
    ;Encoder_Switches
    ; ^F15::  return
    ; ^+F15:: return
    ; ^F16::  return
    ; ^+F16::  return
    ^F17::  Send "{Media_Play_Pause}"
    ^+F17:: Send "{Volume_Mute}"
    ; ^F18::  return
    ; ^+F18:: return
    ;Encoder_Rotation
    >!F15::  Send "{WheelRight}" ;Scroll Right
    >!+F15:: Send "{WheelRight 2}" ;Scroll Right Fast
    ; >!F19::  return
    ; >!+F19:: return
    <!F15::  Send "{WheelLeft}" ;Scroll Left
    <!+F15:: Send "{WheelLeft 2}" ;Scroll Left Fast
    ; <!F19::  return
    ; <!+F19:: return
    >!F16::  Send "^{WheelUp}" ;Zoom in
    ; >!+F16:: return
    ; >!F20::  return
    ; >!+F20:: return
    <!F16::  Send "^{WheelDown}" ;Zoom Out
    ; <!+F16:: return
    ; <!F20::  return
    ; <!+F20:: return
    >!F17::  Send "{Volume_Up}"
    >!+F17:: Send "{Volume_Up 2}"
    >!F21::  Send "{Media_Next}"
    >!+F21:: Send "{Media_Next}"
    <!F17::  Send "{Volume_Down}"
    <!+F17:: Send "{Volume_Down 2}"
    <!F21::  Send "{Media_Prev}"
    <!+F21:: Send "{Media_Prev}"
    ; >!F18::  return
    ; >!+F18:: return
    ; >!F22::  return
    ; >!+F22:: return
    ; <!F18::  return
    ; <!+F18:: return
    ; <!F22::  return
    ; <!+F22:: return

;Firefox
#HotIf WinActive("ahk_exe firefox.exe")
    ;Switches
    F13:: return
    F14:: return
    F15:: Send "^h" ;History
    F16:: Send "^b" ;Bookmarks
    F17:: Send "^!w" ;TabStash
    F18:: Send "^n" ;New Window
    F19:: return
    F20:: Send "^+m" ;Responsive
    F21:: Send "^+k" ;Console
    F22:: Send "^+c" ;Inspector
    F23:: Send "^d" ;Bookmark Tab
    F24:: Send "!{Home}" ;Responsive
    ^F13:: Send "^!s" ;Stashed Tab Sidebar
    ^F14:: Send "^q" ;Quit

;Desktop
#HotIf ActivePage = Desktop
    ;Swithces
    F13:: global ActivePage := Explorer
    F14:: ToggleWindow("ahk_exe reaper.exe", A_ProgramFiles "\REAPER (x64)\reaper.exe")
    F15:: ToggleWindow("ahk_exe Resolve.exe", A_ProgramFiles "\Blackmagic Design\DaVinci Resolve\Resolve.exe")
    F16:: ToggleWindow("ahk_exe TouchDesigner.exe", A_ProgramFiles "\Derivative\TouchDesigner\bin\TouchDesigner.exe")
    F17:: ToggleWindow("ahk_exe Code.exe", A_ProgramFiles "\Microsoft VS Code\Code.exe")
    F18:: ToggleWindow("ahk_exe firefox.exe", A_ProgramFiles "\Firefox Developer Edition\firefox.exe")
    F19:: Send "#2" ;Control Center
    F20:: global ActivePage := Graphic
    F21:: global ActivePage := Coding
    ; F22:: return
    ; F23:: return
    ; F24:: return
    ; ^F13:: return
    ^F14:: Run A_AppData "\Spotify\Spotify.exe"

#HotIf ActivePage = Explorer
    ;Switches
    F13:: { ;File Explorer
        if WinExist("ahk_exe explorer.exe")
        {
            if WinActive()
                WinMinimize
            else
                WinActivate
        } else {
            Send "#e"
        }
    }
    F14:: Run("D:\Pictures")
    F15:: Run "D:\Music"
    F16:: Run "D:\Downloads"
    F17:: Run "D:\Documents"
    F18:: Run "D:\Projects"
    F19:: global ActivePage := Desktop
    F20:: Send "^{Space}" ;Preview
    F21:: Send "!{Enter}" ;Properties
    F22:: Send "{F2}" ;Rename
    F23:: Send "^+n" ;New Folder
    F24:: Send "^x"
    ^F13:: Send "^c"
    ^F14:: Send "^v"
    ;Encoder_Switches
    ^F15:: Send "{Enter}"
    ^F16:: Send "^t"
    ;Encoder_Rotation
    >!F15:: Send "{Down}" ;Move Up
    >!+F15:: Send "{Right}" ;Move Right
    <!F15:: Send "{Up}" ;Move Down
    <!+F15:: Send "{Left}" ;Move Left
    >!+F16:: Send "^{Tab}" ;Next Tab
    <!+F16:: Send "^+{Tab}" ;Prev Tab

#HotIf ActivePage = Graphic
    ;Switches
    ^F12:: ToggleWindow("ahk_exe kicad.exe", A_ProgramFiles "\KiCad\6.0\bin\kicad.exe")
    F14:: ToggleWindow("ahk_exe FusionLauncher.exe", "C:\Users\" A_UserName "\AppData\Local\Autodesk\webdeploy\production\6a0c9611291d45bb9226980209917c3d\FusionLauncher.exe")
    F15:: ToggleWindow("ahk_exe SketchUp.exe", A_ProgramFiles "\SketchUp\SketchUp 2017\SketchUp.exe")
    F16:: ToggleWindow("ahk_exe UnrealEngine.exe", A_ProgramFiles "(x86)\Epic Games\Launcher\Portal\Binaries\Win32\EpicGamesLauncher.exe") ;Unreal Engine
    F17:: ToggleWindow("ahk_exe blender-launcher.exe", A_ProgramFiles "\Blender Foundation\Blender 3.4\blender-launcher.exe")
    F18:: ToggleWindow("ahk_exe TouchDesigner.exe", A_ProgramFiles "\Derivative\TouchDesigner\bin\TouchDesigner.exe")
    F19:: ToggleWindow("ahk_exe lightroom.exe", A_ProgramFiles "\Adobe\Adobe Lightroom CC\lightroom.exe")
    F20:: ToggleWindow("ahk_exe Illustrator.exe", A_ProgramFiles "\Adobe\Adobe Illustrator 2023\Support Files\Contents\Windows\Illustrator.exe")
    F21:: global ActivePage := Desktop
    F22:: ToggleWindow("ahk_exe Adobe Media Encoder.exe", A_ProgramFiles "\Adobe\Adobe Media Encoder 2023\Adobe Media Encoder.exe")
    F23:: ToggleWindow("ahk_exe Adobe Premiere Pro.exe", A_ProgramFiles "\Adobe\Adobe Premiere Pro 2023\Adobe Premiere Pro.exe")
    F24:: ToggleWindow("ahk_exe Resolve.exe", A_ProgramFiles "\Blackmagic Design\DaVinci Resolve\Resolve.exe")
    ^F13:: return
    ^F14:: return
    
;VsCode | Arduino | Coding
#HotIf ActivePage = Coding
    ;Switches
    F13:: { ;Terminal
        Send "#r"
        WinWaitActive 
        Send "wt"
        Sleep 50
        Send "{Enter}"
    }
    F14:: Send "{F2}" ;Rename
    ; F15:: Send "^+k" ;Delete Line
    F16:: Send "^{F2}" ;Select all occurences of current word
    F17:: Send "!{F12}" ;Peek Definition
    F18:: Send "^!f" ;Format Code
    F19:: global ActivePage := Desktop
    F20:: return
    F21:: ToggleWindow("ahk_exe Arduino IDE.exe", A_ProgramFiles "\Arduino\Arduino IDE\Arduino IDE.exe")
    F22:: Send "^+g" ;Git
    F23:: Send "^+e" ;Explorer
    F24:: Send "^+u" ;Output
    ^F13:: Send "^!+s" ;Serial Monitor
    ; ^F14:: Send "^`" ;VsCode Terminal
    ;Encoder_Switches
    ^F17:: { ;Open First Editor in Next Group
        Send "^0" 
        Sleep 10 
        Send "^{PageDown}"
    }
    ;Encoder_Rotation
    >!F15:: Send "{Right}"   ;Move Right
    <!F15:: Send "{Left}"    ;Move Left
    >!F16:: Send "^k{PageUp}" ;Cycle Editors in Group Left
    <!F16:: Send "^k{PageDown}" ;Cycle Editors in Group Right

#HotIf