#Requires AutoHotkey v2.0  ; Recommended for performance and compatibility with future AutoHotkey releases.
#SingleInstance Force
SendMode "Input"
SetWorkingDir A_ScriptDir
A_MenuMaskKey := "vkFF" ; vkFF is no mapping.

; Global
isShiftMode := false
isAutoProfileSwitch := false
isOptionChanged := false
currentTab := 1
currentProfile := "Default"
iniName := ""

; GUI - Base
ControlPanel := Gui("+MinSize400x400 +Resize +AlwaysOnTop", "MacroDeck - Control Panel",)
MenuTab := ControlPanel.AddTab3("x+8 y+16 -Wrap Choose1", ["Base", "Shift", "Encoders", "Profiles", "About"])
; MenuTab.OnEvent("Change", menuTab_Change)

; GUI - Switches
Switches := Map()
Switches["Base"] := []
Switches["Shift"] := []
MenuTab.UseTab("Base")
ControlPanel.AddText("y+16", "Base behaviour - PLACEHOLDER")
SubmitBasic_Sw := ControlPanel.AddButton("xp+958 yp vSubmit_Sw_Base", "Update")
SubmitBasic_Sw.OnEvent("Click", saveOptions_CurrentView)
Loop 16
{
    createGUI_Sw(A_Index, "Base")
}
MenuTab.UseTab("Shift")
ControlPanel.AddText("y+16", "Action fired in shift mode - PLACEHOLDER")
SubmitShift_Sw := ControlPanel.AddButton("xp+958 yp vSubmit_Sw_Shift", "Update")
SubmitShift_Sw.OnEvent("Click", saveOptions_CurrentView)
Loop 16
{
    createGUI_Sw(A_Index, "Shift")
}

; GUI - Encoders
Encoders := Map()
Encoders["Base"] := []
Encoders["Shift"] := []
MenuTab.UseTab("Encoders")
ControlPanel.AddText("y+16", "Base - Select Clockwise (CW), CounterClockwise (CCW), and Push to set multiple keybindings. Contrary to switches, the dropdown list does not limit the action to be fired to the one visible.")
SubmitBasic_Re := ControlPanel.AddButton("xp+958 yp vSubmit_Re_Base", "Update")
SubmitBasic_Re.OnEvent("Click", saveOptions_CurrentView)
Loop 4
{
    createGUI_Re(A_Index, "Base")
}
ControlPanel.AddText("x32 y+48", "Shift")
Loop 4
{
    createGUI_Re(A_Index, "Shift")
}

; GUI - Profiles
MenuTab.UseTab("Profiles")
ControlPanel.AddText("y+16 section", "Profile")
Profile_DDL := ControlPanel.AddDropDownList("x+16 yp Choose1", ["Default", "Profile1"])
Profile_DDL.OnEvent("Change", Profile_DDL_Change)
LimitApplication_CB := ControlPanel.AddCheckbox("xs y+20", "Limit to application: ")
LimitApplication_ED := ControlPanel.AddEdit("x+0 yp-3 -Wrap")
AutoProfileSwitch_CB := ControlPanel.AddCheckbox("xs y+8", "Enable automatic profile switching")

buildNameINI()
updateINI("Settings", "currentProfile", currentProfile, "")
getAllOptions()

ControlPanel.Show()

OnExit promptSave

;-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

; GUI
setHotkeysVisible_Sw(value, state, idx)
{
    Switches[state][idx]["TextHK_Sw"].Visible := value
    Switches[state][idx]["Hotkeys_Sw"].Visible := value
    Switches[state][idx]["TextKP_Sw"].Visible := value
    Switches[state][idx]["Keystrokes_Sw"].Visible := value
}

setWindowVisible_Sw(value, state, idx)
{
    Switches[state][idx]["TextWN_Sw"].Visible := value
    Switches[state][idx]["Window_Sw"].Visible := value
    Switches[state][idx]["TextPR_Sw"].Visible := value
    Switches[state][idx]["PathToExe_Sw"].Visible := value
}

setTogglesVisible_Sw(value, state, idx)
{
    Switches[state][idx]["ShiftMode_Sw"].Visible := value
    Switches[state][idx]["AutoProfile_Sw"].Visible := value
    Switches[state][idx]["ShowProfiles_Sw"].Visible := value
    Switches[state][idx]["ModifiersTG_Sw"].Visible := value
    Switches[state][idx]["ModCtrl_Sw"].Visible := value
    Switches[state][idx]["ModAlt_Sw"].Visible := value
    Switches[state][idx]["ModShift_Sw"].Visible := value
}

setHotkeysVisible_Re(value, dir, state, idx)
{
    Encoders[state][idx]["Label_" dir].Visible := value
    Encoders[state][idx]["TextHK_" dir].Visible := value
    Encoders[state][idx]["TextHK_" dir].Visible := value
    Encoders[state][idx]["Hotkeys_" dir].Visible := value
    Encoders[state][idx]["TextKP_" dir].Visible := value
    Encoders[state][idx]["Keystrokes_" dir].Visible := value
}

createGUI_Sw(idx, state)
{
    Sw := Map()
    if (idx == 1)
        ControlPanel.AddGroupBox("h144 w240 x28 y+11", `
            "Switch " idx`
        )
    else if (idx < 5)
        ControlPanel.AddGroupBox("h144 w240 x28 y+23", `
            "Switch " idx`
        )
    else if (idx == 5)
        ControlPanel.AddGroupBox("h144 w240 x284 y87", `
            "Switch " idx`
        )
    else if (idx < 9)
        ControlPanel.AddGroupBox("h144 w240 x284 y+23", `
            "Switch " idx`
        )
    else if (idx == 9)
        ControlPanel.AddGroupBox("h144 w240 x540 y87", `
            "Switch " idx`
        )
    else if (idx < 13)
        ControlPanel.AddGroupBox("h144 w240 x540 y+23", `
            "Switch " idx`
        )
    else if (idx == 13)
        ControlPanel.AddGroupBox("h144 w240 x796 y87", `
            "Switch " idx`
        )
    else
        ControlPanel.AddGroupBox("h144 w240 x796 y+23", `
            "Switch " idx`
        )

    Sw["ActionType_Sw"] := ControlPanel.AddDropDownList("xp+8 yp+20 w96 Section Choose1 AltSubmit", ["Hotkey", "Window", "Toggle"])
    Sw["ActionType_Sw"].Name := "ActionType_Sw_" state "_" idx
    Sw["ActionType_Sw"].onEvent("Change", ActionType_Sw_Change)
    Sw["Label_Sw"] := ControlPanel.AddEdit("yp -Wrap")
    Sw["Label_Sw"].Name := "Label_Sw_" state "_" idx
    Sw["Label_Sw"].onEvent("Change", handleChange)
    Sw["TextHK_Sw"] := ControlPanel.AddText("xs+4 ys+50 +BackgroundTrans", "Hotkey:")
    Sw["Hotkeys_Sw"] := ControlPanel.AddHotkey("x+8 yp-3")
    Sw["Hotkeys_Sw"].Name := "Hotkeys_Sw_" state "_" idx
    Sw["Hotkeys_Sw"].onEvent("Change", handleChange)
    Sw["TextKP_Sw"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Keystrokes:")
    Sw["Keystrokes_Sw"] := ControlPanel.AddEdit("x+9 yp-3 w154 -Wrap")
    Sw["Keystrokes_Sw"].Name := "Keystrokes_Sw_" state "_" idx
    Sw["Keystrokes_Sw"].onEvent("Change", handleChange)
    Sw["TextWN_Sw"] := ControlPanel.AddText("xs+4 ys+50 +BackgroundTrans", "Window:")
    Sw["Window_Sw"] := ControlPanel.AddEdit("x+8 yp-3 w168 -Wrap")
    Sw["Window_Sw"].Name := "Window_Sw_" state "_" idx
    Sw["Window_Sw"].onEvent("Change", handleChange)
    Sw["TextPR_Sw"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Process:")
    Sw["PathToExe_Sw"] := ControlPanel.AddEdit("x+9 yp-3 w168 -Wrap")
    Sw["PathToExe_Sw"].Name := "PathToExe_Sw_" state "_" idx
    Sw["PathToExe_Sw"].onEvent("Change", handleChange)
    Sw["ShiftMode_Sw"] := ControlPanel.AddRadio("xs+4 ys+34", "Shift Mode")
    SW["ShiftMode_Sw"].Name := "ShiftMode_Sw_" state "_" idx
    SW["ShiftMode_Sw"].onEvent("Click", handleChange)
    Sw["AutoProfile_Sw"] := ControlPanel.AddRadio("xs+4 y+8", "Auto Profile Switching")
    Sw["AutoProfile_Sw"].Name := "AutoProfile_Sw_" state "_" idx
    Sw["AutoProfile_Sw"].onEvent("Click", handleChange)
    Sw["ShowProfiles_Sw"] := ControlPanel.AddRadio("xs+4 y+8", "Show Profiles")
    Sw["ShowProfiles_Sw"].Name := "ShowProfiles_Sw_" state "_" idx
    Sw["ShowProfiles_Sw"].onEvent("Click", handleChange)
    Sw["ModifiersTG_Sw"] := ControlPanel.AddRadio("xs+4 y+8", "Modifiers")
    Sw["ModifiersTG_Sw"].Name := "ModifiersTG_Sw_" state "_" idx
    Sw["ModifiersTG_Sw"].onEvent("Click", handleChange)
    Sw["ModCtrl_Sw"] := ControlPanel.AddCheckBox("xs+72 yp", "Ctrl")
    Sw["ModCtrl_Sw"].Name := "ModCtrl_Sw_" state "_" idx
    Sw["ModCtrl_Sw"].onEvent("Click", handleChange)
    Sw["ModAlt_Sw"] := ControlPanel.AddCheckBox("x+4 yp", "Alt")
    Sw["ModAlt_Sw"].Name := "ModAlt_Sw_" state "_" idx
    Sw["ModAlt_Sw"].onEvent("Click", handleChange)
    Sw["ModShift_Sw"] := ControlPanel.AddCheckBox("x+4 yp", "Shift")
    Sw["ModShift_Sw"].Name := "ModShift_Sw_" state "_" idx
    Sw["ModShift_Sw"].onEvent("Click", handleChange)
    Switches[state].Push(Sw)

    setHotkeysVisible_Sw(true, state, idx)
    setWindowVisible_Sw(false, state, idx)
    setTogglesVisible_Sw(false, state, idx)
}

createGUI_Re(idx, state)
{

    Re := Map()

    (idx == 1) ? ControlPanel.AddGroupBox("h144 w240 x28 y+11", `
        "Encoder " idx`
    ) : ControlPanel.AddGroupBox("h144 w240 x+26 yp-105", `
        "Encoder " idx`
    )
    Re["ActionType_Re"] := ControlPanel.AddDropDownList("xp+8 yp+20 w96 Section Choose1 AltSubmit", ["CW", "CCW", "Push"])
    Re["ActionType_Re"].Name := "ActionType_Re_" state "_" idx
    Re["ActionType_Re"].onEvent("Change", ActionType_Re_Change)
    Re["TextLB_CW"] := ControlPanel.AddText("xs+4 ys+36", "Label:")
    Re["Label_CW"] := ControlPanel.AddEdit("x+15 yp-3 -Wrap")
    Re["Label_CW"].Name := "Label_CW_" state "_" idx
    Re["TextHK_CW"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Hotkey:")
    Re["Hotkeys_CW"] := ControlPanel.AddHotkey("x+8 yp-3")
    Re["Hotkeys_CW"].Name := "Hotkeys_CW_" state "_" idx
    Re["TextKP_CW"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Keystrokes:")
    Re["Keystrokes_CW"] := ControlPanel.AddEdit("x+9 yp-3 w154 -Wrap")
    Re["Keystrokes_CW"].Name := "Keystrokes_CW_" state "_" idx
    Re["TextLB_CCW"] := ControlPanel.AddText("xs+4 ys+36", "Label:")
    Re["Label_CCW"] := ControlPanel.AddEdit("x+15 yp-3 -Wrap")
    Re["Label_CCW"].Name := "Label_CCW_" state "_" idx
    Re["TextHK_CCW"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Hotkey:")
    Re["Hotkeys_CCW"] := ControlPanel.AddHotkey("x+8 yp-3")
    Re["Hotkeys_CCW"].Name := "Hotkeys_CCW_" state "_" idx
    Re["TextKP_CCW"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Keystrokes:")
    Re["Keystrokes_CCW"] := ControlPanel.AddEdit("x+9 yp-3 w154 -Wrap")
    Re["Keystrokes_CCW"].Name := "Keystrokes_CCW_" state "_" idx
    Re["TextLB_PS"] := ControlPanel.AddText("xs+4 ys+36", "Label:")
    Re["Label_PS"] := ControlPanel.AddEdit("x+15 yp-3 -Wrap")
    Re["Label_PS"].Name := "Label_PS_" state "_" idx
    Re["TextHK_PS"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Hotkey:")
    Re["Hotkeys_PS"] := ControlPanel.AddHotkey("x+8 yp-3")
    Re["Hotkeys_PS"].Name := "Hotkeys_PS_" state "_" idx
    Re["TextKP_PS"] := ControlPanel.AddText("xs+4 y+8 +BackgroundTrans", "Keystrokes:")
    Re["Keystrokes_PS"] := ControlPanel.AddEdit("x+9 yp-3 w154 -Wrap")
    Re["Keystrokes_PS"].Name := "Keystrokes_PS_" state "_" idx
    Encoders[state].Push(Re)

    setHotkeysVisible_Re(true, "CW", state, idx)
    setHotkeysVisible_Re(false, "CCW", state, idx)
    setHotkeysVisible_Re(false, "PS", state, idx)
}

; Events
ActionType_Sw_Change(GuiCtrlObj, info?)
{
    target := getInfo(GuiCtrlObj.Name)
    Switch GuiCtrlObj.Value
    {
        Case 1:
            setHotkeysVisible_Sw(true, target.state, target.idx)
            setWindowVisible_Sw(false, target.state, target.idx)
            setTogglesVisible_Sw(false, target.state, target.idx)

        Case 2:
            setHotkeysVisible_Sw(false, target.state, target.idx)
            setWindowVisible_Sw(true, target.state, target.idx)
            setTogglesVisible_Sw(false, target.state, target.idx)

        Case 3:
            setHotkeysVisible_Sw(false, target.state, target.idx)
            setWindowVisible_Sw(false, target.state, target.idx)
            setTogglesVisible_Sw(true, target.state, target.idx)

        Default:
            setHotkeysVisible_Sw(true, target.state, target.idx)
            setWindowVisible_Sw(false, target.state, target.idx)
            setTogglesVisible_Sw(false, target.state, target.idx)
    }
}

ActionType_Re_Change(GuiCtrlObj, Info?)
{
    target := getInfo(GuiCtrlObj.Name)
    Switch GuiCtrlObj.Value
    {
        Case 1:
            setHotkeysVisible_Re(true, "CW", target.state, target.idx)
            setHotkeysVisible_Re(false, "CCW", target.state, target.idx)
            setHotkeysVisible_Re(false, "PS", target.state, target.idx)

        Case 2:
            setHotkeysVisible_Re(false, "CW", target.state, target.idx)
            setHotkeysVisible_Re(true, "CCW", target.state, target.idx)
            setHotkeysVisible_Re(false, "PS", target.state, target.idx)

        Case 3:
            setHotkeysVisible_Re(false, "CW", target.state, target.idx)
            setHotkeysVisible_Re(false, "CCW", target.state, target.idx)
            setHotkeysVisible_Re(true, "PS", target.state, target.idx)

        Default:
            setHotkeysVisible_Re(true, "CW", target.state, target.idx)
            setHotkeysVisible_Re(false, "CCW", target.state, target.idx)
            setHotkeysVisible_Re(false, "PS", target.state, target.idx)
    }
}

Profile_DDL_Change(GuiCtrlObj, Info?)
{
    global currentProfile := GuiCtrlObj.Text
    getAllOptions()
    saveOptions_All()
}

; menuTab_Change(GuiCtrlObj, Info?)
; {
;     if (isOptionChanged)
;     {
;         global currentTab
;         newTab := GuiCtrlObj.Value
;         GuiCtrlObj.Value := currentTab
;         Res := MsgBox("You made some changes. Would you like to save them?", "Unsaved changes" GuiCtrlObj.Text, "Y/N/C")
;         Ctrl := {}
;         Ctrl.Name := "MenuTab_Tab3_" GuiCtrlObj.Text
;         switch res {
;             case "Yes":
;                 saveOptions_CurrentView(Ctrl)
;                 GuiCtrlObj.Value := newTab
;                 currentTab := newTab
;             case "No":
;                 GuiCtrlObj.Value := newTab
;                 currentTab := newTab
;             default:
;         }
;     }

; }

handleChange(GuiCtrlObj, Info?, *)
{
    optionChanged()
    target := getInfo(GuiCtrlObj.Name)
    switch target.baseName
    {
        case "ModCtrl_Sw", "ModAlt_Sw", "ModShift_Sw":
            targetCtrl := Switches[target.state][target.idx]
            if (targetCtrl["ModCtrl_Sw"].Value || targetCtrl["ModAlt_Sw"].Value || targetCtrl["ModShift_Sw"].Value)
                targetCtrl["ModifiersTG_Sw"].Value := 1

        case "ShiftMode_Sw":
            oppositeState := (target.state == "Base") ? "Shift" : "Base"
            Switches[oppositeState][target.idx]["ShiftMode_Sw"].Value := GuiCtrlObj.Value
            Switches[oppositeState][target.idx]["ActionType_Sw"].Value := 3
            ActionType_Sw_Change(Switches[oppositeState][target.idx]["ActionType_Sw"])

    }
    if (target.state == "Shift")
    {
        isShiftModeTrigger := false
        loop 16
            if (Switches["Base"][A_index]["ShiftMode_Sw"].Value)
                isShiftModeTrigger := true
        if (!isShiftModeTrigger)
            MsgBox "None of the switches toggles ShiftMode.`nYou won't be able to trigger any of the actions specified under 'Shift', unless you assign the ShiftMode toggle action to one of the swithces"
    }

}

promptSave(ExitReason, *)
{
    if (ExitReason != "Error" && isOptionChanged)
    {
        Res := MsgBox("You made some changes. Would you like to save them?", "Unsaved changes", "Y/N/C")
        switch res {
            case "Yes":
                saveOptions_All()
            case "No":
                doubleCheck := MsgBox("All changes will be lost`nAre you sure you want to exit?", "Unsaved changes", "Y/N")
                if (doubleCheck == "No")
                    return 1
            case "Cancel":
                return 1
        }
    }
}

; Helpers
getInfo(name)
{
    out := {}
    tmp := StrSplit(name, "_")
    out.baseName := tmp[1]
    if (tmp.Has(2)) {
        out.type := tmp[2]
        out.baseName := tmp[1] "_" tmp[2]
    }
    if (tmp.Has(3))
        out.state := tmp[3]
    if (tmp.Has(4))
        out.idx := tmp[4]
    return out
}

optionChanged(*)
{
    global isOptionChanged
    if (!isOptionChanged)
        isOptionChanged := true
}

; INI

buildNameINI()
{
    global iniName
    iniName_tmp := ""
    last := ""
    tmp := StrSplit(A_ScriptName, ".")
    loop tmp.Length
    {
        if (last != "")
        {
            if (iniName_tmp != "")
            {
                iniName_tmp := iniName_tmp "."
            }
            iniName_tmp := iniName_tmp last
        }
        last := tmp[A_Index]
    }
    iniName := iniName_tmp ".ini"
}

; Updates the settings file. If value is default, it deletes the setting to keep the file as tidy as possible
updateINI(section, key, value, default)
{
    if (value != default)
    {   ; Only write the value if it differs from what is already written
        if (IniRead(iniName, section, key, -1) != value)
            IniWrite(value, iniName, section, key)
    }
    else
    {   ; Only delete the value if there is already a value to delete
        if (IniRead(iniName, section, key, -1) != -1)
            IniDelete(iniName, section, key)
    }
}

/* TODO add checks
* like:
*   case "Window_Sw":
*       if (Switches[target.state][target.idx]["PathToExe_Sw"].Value := "")
*           MsgBox "Path to program missing"
*   case "PathToExe_Sw":
*       if (Switches[target.state][target.idx]["PathToExe_Sw"].Value := "")
*           MsgBox "Window undefined`nWindow selection won't work unless you define a window"
*/

saveCtrl(Ctrl)
{
    for each, GuiCtrlObj in Ctrl
    {
        if (GuiCtrlObj.Name)
            if (GuiCtrlObj.Type == "Edit" || GuiCtrlObj.Type == "Hotkey")
                updateINI(currentProfile, GuiCtrlObj.Name, GuiCtrlObj.Value, "")
            else if (GuiCtrlObj.Type == "DDL")
                updateINI(currentProfile, GuiCtrlObj.Name, GuiCtrlObj.Value, 1)
            else
                updateINI(currentProfile, GuiCtrlObj.Name, GuiCtrlObj.Value, 0)
    }
}

saveOptions_CurrentView(GuiCtrlObj, info?)
{
    target := getInfo(GuiCtrlObj.Name)
    switch target.type
    {
        case "Sw":
            loop 16
                saveCtrl(Switches[target.state][A_Index])

        case "Re":
            ; TODO update when option is implemented
            for each, state in Encoders
                loop 4
                    saveCtrl(state[A_Index])

        default:
            ; MsgBox "Unrecognised type: " target.baseName " won't be saved"
    }
    global isOptionChanged := false
}

saveOptions_All(*)
{
    for each, state in Switches
        loop 16
            saveCtrl(state[A_Index])

    for each, state in Encoders
        loop 4
            saveCtrl(state[A_Index])

    global isOptionChanged := false
}

getAllOptions()
{
    global
    local NamedCtrlContents := ControlPanel.Submit(false)
    for GuiCtrlObjName, value in NamedCtrlContents.OwnProps()
    {
        local target := getInfo(GuiCtrlObjName)
        local GuiCtrlObj := {}
        switch target.type
        {
            case "Sw":
                GuiCtrlObj := Switches[target.state][target.idx][target.baseName]
            case "Re", "CW", "CCW", "PS":
                GuiCtrlObj := Encoders[target.state][target.idx][target.baseName]
            default:
        }
        if (GuiCtrlObj.Type == "Edit" || GuiCtrlObj.Type == "Hotkey")
            GuiCtrlObj.Value := IniRead(iniName, currentProfile, GuiCtrlObjName, "")
        else
        {
            if (target.baseName == "ActionType_Sw")
            {
                GuiCtrlObj.Value := Integer(IniRead(iniName, currentProfile, GuiCtrlObjName, 1))
                ActionType_Sw_Change(GuiCtrlObj)
            }
            else if (target.baseName == "ActionType_Re")
            {
                GuiCtrlObj.Value := Integer(IniRead(iniName, currentProfile, GuiCtrlObjName, 1))
                ActionType_Re_Change(GuiCtrlObj)
            }
            else
                GuiCtrlObj.Value := Integer(IniRead(iniName, currentProfile, GuiCtrlObjName, 0))
        }
    }
}


; Actions
ToggleWindow(WinClass, PathToExe) {
    if WinExist(WinClass)
    {
        if WinActive()
            WinMinimize
        else
            WinActivate
    } else
        Run PathToExe
}

showProfiles()
{ }

fireAction_Sw(idx)
{
    global isShiftMode
    global isAutoProfileSwitch
    state := (isShiftMode) ? "Shift" : "Base"
    switch Switches[state][idx]["ActionType_Sw"].Value
    {
        case 1:
            if (Switches[state][idx]["Hotkeys_Sw"].Value != "" || Switches[state][idx]["Keystrokes_Sw"].Value != "")
            {
                local toSend := Switches[state][idx]["Hotkeys_Sw"].Value Switches[state][idx]["Keystrokes_Sw"].Value
                Send(toSend)
            }
        case 2:
            if (Switches[state][idx]["Window_Sw"].Value != "" || Switches[state][idx]["PathToExe_Sw"].Value != "")
                ToggleWindow(Switches[state][idx]["Window_Sw"].Value, Switches[state][idx]["PathToExe_Sw"].Value)
        case 3:
            switch Switches[state][idx]["ShiftMode_Sw"].Value
            {
                case 1: isShiftMode := !isShiftMode, "On"
                case 2: isAutoProfileSwitch := !isAutoProfileSwitch, "On"
                case 3: showProfiles()
                case 4:
                    local toSend := ""
                    if (Switches[state][idx]["ModCtrl_Sw"].Value)
                        toSend := toSend (GetKeyState("Ctrl")) ? "{Ctrl Up}" : "{Crtl Down}"
                    if (Switches[state][idx]["ModAlt_Sw"].Value)
                        toSend := toSend (GetKeyState("Alt")) ? "{Alt Up}" : "{Alt Down}"
                    if (Switches[state][idx]["ModShift_Sw"].Value)
                        toSend := toSend (GetKeyState("Shift")) ? "{Shift Up}" : "{Shift Down}"
                    Send(toSend)
            }
    }
}

fireAction_Re(dir, idx)
{
    state := (isShiftMode) ? "Shift" : "Base"
    toSend := Encoders[state][idx][Hotkeys_%dir%].Value Encoders[state][idx][Keystrokes_%dir%].Value
    Send(toSend)
}

; HotKeys - Switches
+F12:: getAllOptions()
F12:: fireAction_Sw(1)
; F14:: fireAction_Sw(2)
; F15:: fireAction_Sw(3)
; F16:: fireAction_Sw(4)
; F17:: fireAction_Sw(5)
; F18:: fireAction_Sw(6)
; F19:: fireAction_Sw(7)
; F20:: fireAction_Sw(8)
; F21:: fireAction_Sw(9)
; F22:: fireAction_Sw(10)
; F23:: fireAction_Sw(11)
; F24:: fireAction_Sw(12)
; ^F13:: fireAction_Sw(13)
; ^F14:: fireAction_Sw(14)
; ^F15:: fireAction_Sw(15)
; ^F16:: fireAction_Sw(16)

; HotKeys - Encoder_Switches
; ^F17:: fireAction_Re("PS", 1)
; ^F18:: fireAction_Re("PS", 2)
; ^F19:: fireAction_Re("PS", 3)
; ^F20:: fireAction_Re("PS", 4)
; HotKeys - Encoder_Rotation
; >F17:: fireAction_Re("CW", 1)
; <F17:: fireAction_Re("CCW", 1)
; >F18:: fireAction_Re("CW", 2)
; <F18:: fireAction_Re("CCW", 2)
; >F19:: fireAction_Re("CW", 3)
; <F19:: fireAction_Re("CCW", 3)
; >F20:: fireAction_Re("CW", 4)
; <F20:: fireAction_Re("CCW", 4)
