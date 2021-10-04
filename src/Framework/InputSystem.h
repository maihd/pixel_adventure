#pragma once

#include <stdint.h>
#include <stdbool.h>

namespace MouseButtons
{
    enum Type : uint32_t
    {
        None        = 0,
        Left        = 1 << 0,
        Middle      = 1 << 1,
        Right       = 1 << 2,
        XButton1    = 1 << 3,
        XButton2    = 1 << 4,
    };
}
using MouseButton = MouseButtons::Type;

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

enum struct KeyCode : uint32_t
{
    None,

    Backspace = '\b',
    Tab = '\t',

    Return = '\r',
    Escape = 27,
    Space = ' ',

    A = 'a', 
    B, 
    C, 
    D, 
    E, 
    F, 
    G, 
    H, 
    I, 
    J, 
    K, 
    L, 
    M,
    N, 
    O, 
    P, 
    Q, 
    R, 
    S, 
    T, 
    U, 
    V, 
    W, 
    X, 
    Y, 
    Z = 'z',

    Keypad0 = 48, 
    Keypad1, 
    Keypad2, 
    Keypad3, 
    Keypad4, 
    Keypad5, 
    Keypad6, 
    Keypad7, 
    Keypad8, 
    Keypad9,

    Numpad0 = 96, 
    Numpad1, 
    Numpad2, 
    Numpad3, 
    Numpad4, 
    Numpad5, 
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,

    NumpadMultiply = 106,
    NumpadAdd = 107,
    NumpadSubtract = 109,
    NumpadPeriod = 110,
    NumpadDivide = 111,
    NumpadEnter = 112,

    Colon = ':',
    SemiColon = ';',
    Comma = ',',
    Period = '.',
    Slash = '/',
    BackSlash = '\\',
    Quote = '\'',
    DoubleQuote = '\"',

    At = '@',
    Tilde = '~',
    Plus = '+',
    Minus = '-',
    Caret = '^',
    Aterisk = '*',
    Percent = '%',
    Dolla = '$',
    Hash = '#',
    Exclaim = '!',
    Ampersand = '&',
    Question = '?',
    Underscore = '_',
    BackQuote = '`',

    LeftParen = '(',
    RightParen = ')',

    LeftBracket = '[',
    RightBracket = ']',

    LeftCurlyBracket = '{',
    RightCurlyBracket = '}',

    Less = '<',
    Equal = '=',
    Greater = '>',

    CapsLock,
    NumLock,
    ScrollLock,

    Insert,
    Delete,
    Home,
    End,
    PageUp,
    PageDown,

    LeftArrow,
    RightArrow,
    UpArrow,
    DownArrow,

    Pause, Break = Pause,

    LeftAlt,
    LeftControl,
    LeftShift,

    RightAlt,
    RightControl,
    RightShift,

    Menu,
    Application,
    PrintScreen,

    LeftWindows,
    RightWindows,

    F1, 
    F2, 
    F3, 
    F4, 
    F5, 
    F6, 
    F7, 
    F8, 
    F9, 
    F10, 
    F11, 
    F12,
    F13,
    F14, 
    F15, 
    F16, 
    F17, 
    F18, 
    F19, 
    F20, 
    F21, 
    F22, 
    F23, 
    F24,
};

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
