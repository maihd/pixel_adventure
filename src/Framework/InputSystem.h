#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum MouseButton
{
    MouseButton_None        = 0,
    MouseButton_Left        = 1 << 0,
    MouseButton_Middle      = 1 << 1,
    MouseButton_Right       = 1 << 2,
    MouseButton_XButton1    = 1 << 3,
    MouseButton_XButton2    = 1 << 4,
} MouseButton;

typedef enum GamepadAxis
{
    GamepadAxis_None = -1,

    GamepadAxis_LeftVertical,
    GamepadAxis_LeftHorizontal,

    GamepadAxis_RightVertical,
    GamepadAxis_RightHorizontal,

    GamepadAxis_LeftTrigger,
    GamepadAxis_RightTrigger,
} GamepadAxis;

typedef enum GamepadButton
{
    GamepadButton_None = -1,

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

    GamepadButton_Menu = GamepadButton_Start,
    GamepadButton_Map = GamepadButton_Back,

    GamepadButton_A = GamepadButton_Cross,
    GamepadButton_B = GamepadButton_Circle,
    GamepadButton_X = GamepadButton_Square,
    GamepadButton_Y = GamepadButton_Triangle,
} GamepadButton;

typedef enum KeyCode
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

void            InputSystem_Setup(void);
void            InputSystem_Shutdown(void);

void            InputSystem_NewFrame(void);
void            InputSystem_EndFrame(void);

void            InputSystem_UpdateCharInput(int character);
void            InputSystem_UpdateTextInput(const char* string);

void            InputSystem_UpdateKey(KeyCode key, bool down);

void            InputSystem_UpdateMouse(MouseButton button, bool down);
void            InputSystem_UpdateMouseMove(float x, float y);
void            InputSystem_UpdateMouseWheel(float h, float v);

void            InputSystem_UpdateGamepadAxis(int gamepadID, GamepadAxis axis, float value);
void            InputSystem_UpdateGamepadButton(int gamepadID, GamepadButton button, bool down);

void            InputSystem_ShowMouseCursor(void);
void            InputSystem_HideMouseCursor(void);

bool            InputSystem_IsMouseCursorVisibled(void);
void            InputSystem_SetMouseCursorVisible(bool visible);

int             InputSystem_GetMouseState(float* x, float* y);

bool            InputSystem_GetMouseButton(MouseButton button);
bool            InputSystem_GetMouseButtonUp(MouseButton button);
bool            InputSystem_GetMouseButtonDown(MouseButton button);

const char*     InputSystem_GetTextInput(void);

bool            InputSystem_GetKey(KeyCode button);
bool            InputSystem_GetKeyUp(KeyCode button);
bool            InputSystem_GetKeyDown(KeyCode button);

bool            InputSystem_IsGamepadAttached(int gamepadID);
float           InputSystem_GetAxis(int gamepadID, GamepadAxis axis);
bool            InputSystem_GetButton(int gamepadID, GamepadButton button);
bool            InputSystem_GetButtonUp(int gamepadID, GamepadButton button);
bool            InputSystem_GetButtonDown(int gamepadID, GamepadButton button);

#ifdef __cplusplus
}
#endif
