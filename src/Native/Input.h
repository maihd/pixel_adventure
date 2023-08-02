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

enum struct GamepadAxis : uint32_t
{
    LeftVertical,
    LeftHorizontal,

    RightVertical,
    RightHorizontal,

    LeftTrigger,
    RightTrigger,
};

enum struct GamepadButton : uint32_t
{
    Cross,
    Circle,
    Square,
    Triangle,

    Back,
    Guide,
    Start,

    LeftStick,
    RightStick,

    LeftBumper,
    RightBumper,

    DpadUp,
    DpadDown,
    DpadLeft,
    DpadRight,

    Menu        = Start,
    Map         = Back,

    A           = Cross,
    B           = Circle,
    X           = Square,
    Y           = Triangle,

    SwitchPlus  = Start,
    SwitchMinus = Back,

    SwitchA     = Circle,
    SwitchB     = Cross,
    SwitchX     = Triangle,
    SwitchY     = Square,

    SwitchHome      = DpadRight + 1,
    SwitchCapture   = SwitchHome + 1,
};

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

namespace Input
{
    void            Setup(void);
    void            Shutdown(void);
    
    void            NewFrame(void);
    void            EndFrame(void);
    
    void            UpdateCharInput(int character);
    void            UpdateTextInput(const char* string);
    
    void            UpdateKey(KeyCode key, bool down);
    
    void            UpdateMouse(MouseButton button, bool down);
    void            UpdateMouseMove(float x, float y);
    void            UpdateMouseWheel(float h, float v);
    
    void            UpdateGamepadAxis(int gamepadId, GamepadAxis axis, float value);
    void            UpdateGamepadButton(int gamepadId, GamepadButton button, bool down);
    
    void            ShowMouseCursor(void);
    void            HideMouseCursor(void);
    
    bool            IsMouseCursorVisibled(void);
    void            SetMouseCursorVisible(bool visible);
    
    int             GetMouseState(float* x, float* y);
    
    bool            GetMouseButton(MouseButton button);
    bool            GetMouseButtonUp(MouseButton button);
    bool            GetMouseButtonDown(MouseButton button);
    
    const char*     GetTextInput(void);
    
    bool            GetKey(KeyCode button);
    bool            GetKeyUp(KeyCode button);
    bool            GetKeyDown(KeyCode button);
    
    bool            IsGamepadAttached(int32_t gamepadId);

    float           GetAxis(int32_t gamepadId, GamepadAxis axis);
    bool            GetButton(int32_t gamepadId, GamepadButton button);
    bool            GetButtonUp(int32_t gamepadId, GamepadButton button);
    bool            GetButtonDown(int32_t gamepadId, GamepadButton button);

    void            Bind(uint32_t action, KeyCode key);
    void            Bind(uint32_t action, MouseButton mouseButton);
    void            Bind(uint32_t action, GamepadButton button);
}
