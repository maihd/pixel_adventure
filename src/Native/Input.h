#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "Misc/Compiler.h"

/// MouseButton
typedef uint32_t MouseButton;

/// MouseButtons
/// Mouse button values
enum MouseButtons __enum_type(uint32_t)
{
    MouseButton_None        = 0,
    MouseButton_Left        = 1 << 0,
    MouseButton_Middle      = 1 << 1,
    MouseButton_Right       = 1 << 2,
    MouseButton_XButton1    = 1 << 3,
    MouseButton_XButton2    = 1 << 4,
};

/// GamepadAxis
/// Axes of gamepad/controller
typedef enum GamepadAxis __enum_type(uint32_t)
{
    GamepadAxis_LeftVertical,
    GamepadAxis_LeftHorizontal,

    GamepadAxis_RightVertical,
    GamepadAxis_RightHorizontal,

    GamepadAxis_LeftTrigger,
    GamepadAxis_RightTrigger,
} GamepadAxis;

/// GamepadButton
/// Button of gamepad/controller
typedef enum GamepadButton __enum_type(uint32_t)
{
    // DualShock & DualSense

    GamepadButton_Cross,
    GamepadButton_Circle,
    GamepadButton_Square,
    GamepadButton_Triangle,

    GamepadButton_Back,
    GamepadButton_Guide,
    GamepadButton_Start,

    GamepadButton_LeftStick,
    GamepadButton_RightStick,

    GamepadButton_LeftBumper,
    GamepadButton_RightBumper,

    GamepadButton_DpadUp,
    GamepadButton_DpadDown,
    GamepadButton_DpadLeft,
    GamepadButton_DpadRight,

    // XBox

    GamepadButton_Menu              = GamepadButton_Start,
    GamepadButton_Map               = GamepadButton_Back,

    GamepadButton_A                 = GamepadButton_Cross,
    GamepadButton_B                 = GamepadButton_Circle,
    GamepadButton_X                 = GamepadButton_Square,
    GamepadButton_Y                 = GamepadButton_Triangle,

    // Nintendo Switch

    GamepadButton_SwitchPlus        = GamepadButton_Start,
    GamepadButton_SwitchMinus       = GamepadButton_Back,

    GamepadButton_SwitchA           = GamepadButton_Circle,
    GamepadButton_SwitchB           = GamepadButton_Cross,
    GamepadButton_SwitchX           = GamepadButton_Triangle,
    GamepadButton_SwitchY           = GamepadButton_Square,

    GamepadButton_SwitchHome        = GamepadButton_DpadRight + 1,
    GamepadButton_SwitchCapture     = GamepadButton_SwitchHome + 1,
} GamepadButton;

/// KeyCode
/// Value receive from keyboard (after parsing scancode)
typedef enum KeyCode __enum_type(uint32_t)
{
    KeyCode_None,

    KeyCode_Backspace = '\b',
    KeyCode_Tab = '\t',

    KeyCode_Return = '\r',
    KeyCode_Escape = 27,
    KeyCode_Space = ' ',

    KeyCode_A = 'a', 
    KeyCode_B, 
    KeyCode_C, 
    KeyCode_D, 
    KeyCode_E, 
    KeyCode_F, 
    KeyCode_G, 
    KeyCode_H, 
    KeyCode_I, 
    KeyCode_J, 
    KeyCode_K, 
    KeyCode_L, 
    KeyCode_M,
    KeyCode_N, 
    KeyCode_O, 
    KeyCode_P, 
    KeyCode_Q, 
    KeyCode_R, 
    KeyCode_S, 
    KeyCode_T, 
    KeyCode_U, 
    KeyCode_V, 
    KeyCode_W, 
    KeyCode_X, 
    KeyCode_Y, 
    KeyCode_Z = 'z',

    KeyCode_Keypad0 = 48, 
    KeyCode_Keypad1, 
    KeyCode_Keypad2, 
    KeyCode_Keypad3, 
    KeyCode_Keypad4, 
    KeyCode_Keypad5, 
    KeyCode_Keypad6, 
    KeyCode_Keypad7, 
    KeyCode_Keypad8, 
    KeyCode_Keypad9,

    KeyCode_Numpad0 = 96, 
    KeyCode_Numpad1, 
    KeyCode_Numpad2, 
    KeyCode_Numpad3, 
    KeyCode_Numpad4, 
    KeyCode_Numpad5, 
    KeyCode_Numpad6,
    KeyCode_Numpad7,
    KeyCode_Numpad8,
    KeyCode_Numpad9,

    KeyCode_NumpadMultiply = 106,
    KeyCode_NumpadAdd = 107,
    KeyCode_NumpadSubtract = 109,
    KeyCode_NumpadPeriod = 110,
    KeyCode_NumpadDivide = 111,
    KeyCode_NumpadEnter = 112,

    KeyCode_Colon = ':',
    KeyCode_SemiColon = ';',
    KeyCode_Comma = ',',
    KeyCode_Period = '.',
    KeyCode_Slash = '/',
    KeyCode_BackSlash = '\\',
    KeyCode_Quote = '\'',
    KeyCode_DoubleQuote = '\"',

    KeyCode_At = '@',
    KeyCode_Tilde = '~',
    KeyCode_Plus = '+',
    KeyCode_Minus = '-',
    KeyCode_Caret = '^',
    KeyCode_Aterisk = '*',
    KeyCode_Percent = '%',
    KeyCode_Dolla = '$',
    KeyCode_Hash = '#',
    KeyCode_Exclaim = '!',
    KeyCode_Ampersand = '&',
    KeyCode_Question = '?',
    KeyCode_Underscore = '_',
    KeyCode_BackQuote = '`',

    KeyCode_LeftParen = '(',
    KeyCode_RightParen = ')',

    KeyCode_LeftBracket = '[',
    KeyCode_RightBracket = ']',

    KeyCode_LeftCurlyBracket = '{',
    KeyCode_RightCurlyBracket = '}',

    KeyCode_Less = '<',
    KeyCode_Equal = '=',
    KeyCode_Greater = '>',

    KeyCode_CapsLock,
    KeyCode_NumLock,
    KeyCode_ScrollLock,

    KeyCode_Insert,
    KeyCode_Delete,
    KeyCode_Home,
    KeyCode_End,
    KeyCode_PageUp,
    KeyCode_PageDown,

    KeyCode_LeftArrow,
    KeyCode_RightArrow,
    KeyCode_UpArrow,
    KeyCode_DownArrow,

    KeyCode_Pause, KeyCode_Break = KeyCode_Pause,

    KeyCode_LeftAlt,
    KeyCode_LeftControl,
    KeyCode_LeftShift,

    KeyCode_RightAlt,
    KeyCode_RightControl,
    KeyCode_RightShift,

    KeyCode_Menu,
    KeyCode_Application,
    KeyCode_PrintScreen,

    KeyCode_LeftWindows,
    KeyCode_RightWindows,

    KeyCode_F1, 
    KeyCode_F2, 
    KeyCode_F3, 
    KeyCode_F4, 
    KeyCode_F5, 
    KeyCode_F6, 
    KeyCode_F7, 
    KeyCode_F8, 
    KeyCode_F9, 
    KeyCode_F10, 
    KeyCode_F11, 
    KeyCode_F12,
    KeyCode_F13,
    KeyCode_F14, 
    KeyCode_F15, 
    KeyCode_F16, 
    KeyCode_F17, 
    KeyCode_F18, 
    KeyCode_F19, 
    KeyCode_F20, 
    KeyCode_F21, 
    KeyCode_F22, 
    KeyCode_F23, 
    KeyCode_F24,
} KeyCode;

#ifdef __cplusplus
extern "C" {
#endif

void            Input_Setup(void);
void            Input_Shutdown(void);

void            Input_NewFrame(void);
void            Input_EndFrame(void);

void            Input_UpdateCharInput(int character);
void            Input_UpdateTextInput(const char* string);

void            Input_UpdateKey(KeyCode key, bool down);

void            Input_UpdateMouse(MouseButton button, bool down);
void            Input_UpdateMouseMove(float x, float y);
void            Input_UpdateMouseWheel(float h, float v);

void            Input_UpdateGamepadAxis(int gamepadId, GamepadAxis axis, float value);
void            Input_UpdateGamepadButton(int gamepadId, GamepadButton button, bool down);

void            Input_ShowMouseCursor(void);
void            Input_HideMouseCursor(void);

bool            Input_IsMouseCursorVisibled(void);
void            Input_SetMouseCursorVisible(bool visible);

int             Input_GetMouseState(float* x, float* y);

bool            Input_GetMouseButton(MouseButton button);
bool            Input_GetMouseButtonUp(MouseButton button);
bool            Input_GetMouseButtonDown(MouseButton button);

const char*     Input_GetTextInput(void);

bool            Input_GetKey(KeyCode button);
bool            Input_GetKeyUp(KeyCode button);
bool            Input_GetKeyDown(KeyCode button);

bool            Input_IsGamepadAttached(int32_t gamepadId);

float           Input_GetAxis(int32_t gamepadId, GamepadAxis axis);
bool            Input_GetButton(int32_t gamepadId, GamepadButton button);
bool            Input_GetButtonUp(int32_t gamepadId, GamepadButton button);
bool            Input_GetButtonDown(int32_t gamepadId, GamepadButton button);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
