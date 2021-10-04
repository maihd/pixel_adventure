#include <string.h>

#include "InputSystem.h"

#define COUNTOF(x) (sizeof(x) / sizeof((x)[0]))

typedef struct GamepadState
{
    float   axis[8];

    int     buttonUpFrame[32];
    int     buttonDownFrame[32];
} GamepadState;

enum { KEY_INPUT_COUNT = 1024 };
enum { MOUSE_INPUT_COUNT = 8 };

static int32_t          gCurrentFrame;

static int32_t          gInputTextLength;
static char             gInputText[1024];

static bool             gKeyState[KEY_INPUT_COUNT];
static int32_t          gKeyUpFrame[KEY_INPUT_COUNT];
static int32_t          gKeyDownFrame[KEY_INPUT_COUNT];

static int32_t          gMouseState;
static int32_t          gMouseUpFrame[MOUSE_INPUT_COUNT];
static int32_t          gMouseDownFrame[MOUSE_INPUT_COUNT];
static float            gMouseX;
static float            gMouseY;

static float            gMouseWheelH;
static float            gMouseWheelV;

static GamepadState     gGamepadStates[8];

void InputSystem_Setup(void)
{
    gCurrentFrame = 1;
}

void InputSystem_Shutdown(void)
{

}

void InputSystem_NewFrame(void)
{
    gInputTextLength = 0;
}

void InputSystem_EndFrame(void)
{
    gInputText[gInputTextLength] = 0;
    gCurrentFrame++;
}

void InputSystem_UpdateCharInput(int character)
{
    gInputText[gInputTextLength++] = (char)character;
}

void InputSystem_UpdateTextInput(const char* string)
{
    int32_t length = (int32_t)strlen(string);
    if (gInputTextLength + length >= sizeof(gInputText))
    {
        length = sizeof(gInputText) - gInputTextLength - 1;
    }

    memcpy(gInputText + gInputTextLength, string, length);
    gInputTextLength += length;

    gInputText[gInputTextLength] = '\0';
}

void InputSystem_UpdateKey(KeyCode key, bool down)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        gKeyState[index] = down;

        if (down)
        {
            gKeyDownFrame[index] = gCurrentFrame;
        }
        else
        {
            gKeyUpFrame[index] = gCurrentFrame;
        }
    }
}

void InputSystem_UpdateMouse(MouseButton button, bool down)
{
    int index = (int)button;
    if (index > -1 && index < MOUSE_INPUT_COUNT)
    {
        if (down)
        {
            gMouseState |= button;
            gMouseDownFrame[index] = gCurrentFrame;
        }
        else
        {
            gMouseState &= ~button;
            gMouseUpFrame[index] = gCurrentFrame;
        }
    }
}

void InputSystem_UpdateMouseMove(float x, float y)
{
    gMouseX = x;
    gMouseY = y;
}

void InputSystem_UpdateMouseWheel(float h, float v)
{
    gMouseWheelV += v;
    gMouseWheelH += h;
}

void InputSystem_UpdateGamepadAxis(int gamepadID, GamepadAxis axis, float value)
{
    if (gamepadID > -1 && gamepadID < COUNTOF(gGamepadStates))
    {
        GamepadState* state = &gGamepadStates[gamepadID];

        int index = (int)axis;
        if (index > -1 && index < COUNTOF(state->axis))
        {
            state->axis[index] = value;
        }
    }
}

void InputSystem_UpdateGamepadButton(int gamepadID, GamepadButton button, bool down)
{
    if (gamepadID > -1 && gamepadID < COUNTOF(gGamepadStates))
    {
        GamepadState* state = &gGamepadStates[gamepadID];

        int index = (int)button;
        if (down)
        {
            if (index > -1 && index < COUNTOF(state->buttonDownFrame))
            {
                state->buttonDownFrame[index] = gCurrentFrame;
            }
        }
        else
        {
            if (index > -1 && index < COUNTOF(state->buttonUpFrame))
            {
                state->buttonUpFrame[index] = gCurrentFrame;
            }
        }
    }
}

void InputSystem_ShowMouseCursor(void)
{
    InputSystem_SetMouseCursorVisible(true);
}

void InputSystem_HideMouseCursor(void)
{
    InputSystem_SetMouseCursorVisible(false);
}

bool InputSystem_IsMouseCursorVisibled(void)
{
    return false;
}

void InputSystem_SetMouseCursorVisible(bool visible)
{

}

int InputSystem_GetMouseState(float* x, float* y)
{
    if (x) *x = gMouseX;
    if (y) *y = gMouseY;

    return gMouseState;
}

bool InputSystem_GetMouseButton(MouseButton button)
{
    int index = (int)button;
    if (index > -1 && index < COUNTOF(gMouseDownFrame) && index < COUNTOF(gMouseUpFrame))
    {
        return gMouseDownFrame[index] > gMouseUpFrame[index];
    }

    return false;
}

bool InputSystem_GetMouseButtonUp(MouseButton button)
{
    int index = (int)button;
    if (index > -1 && index < COUNTOF(gMouseUpFrame))
    {
        return gMouseUpFrame[index] == gCurrentFrame;
    }

    return false;
}

bool InputSystem_GetMouseButtonDown(MouseButton button)
{
    int index = (int)button;
    if (index > -1 && index < COUNTOF(gMouseDownFrame))
    {
        return gMouseDownFrame[index] == gCurrentFrame;
    }

    return false;
}

bool InputSystem_GetKey(KeyCode key)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        return gKeyState[index];
    }

    return false;
}

bool InputSystem_GetKeyUp(KeyCode key)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        return gKeyUpFrame[index] == gCurrentFrame;
    }

    return false;
}

bool InputSystem_GetKeyDown(KeyCode key)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        return gKeyDownFrame[index] == gCurrentFrame;
    }

    return false;
}

float InputSystem_GetAxis(int gamepadID, GamepadAxis axis)
{
    if (gamepadID > -1 && gamepadID < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadID];

        int index = (int)axis;
        if (index > -1 && index < COUNTOF(state->axis))
        {
            return state->axis[index];
        }
    }

    return 0.0f;
}

bool InputSystem_GetButton(int gamepadID, GamepadButton button)
{
    if (gamepadID > -1 && gamepadID < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadID];

        int index = (int)button;
        if (index > -1 && index < COUNTOF(state->buttonDownFrame) && index < COUNTOF(state->buttonUpFrame))
        {
            return state->buttonDownFrame[index] > state->buttonUpFrame[index];
        }
    }

    return false;
}

bool InputSystem_GetButtonUp(int gamepadID, GamepadButton button)
{
    if (gamepadID > -1 && gamepadID < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadID];

        int index = (int)button;
        if (index > -1 && index < COUNTOF(state->buttonUpFrame))
        {
            return state->buttonUpFrame[index] == gCurrentFrame;
        }
    }

    return false;
}

bool InputSystem_GetButtonDown(int gamepadID, GamepadButton button)
{
    if (gamepadID > -1 && gamepadID < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadID];

        int index = (int)button;
        if (index > -1 && index < COUNTOF(state->buttonDownFrame))
        {
            return state->buttonDownFrame[index] == gCurrentFrame;
        }
    }

    return false;
}

const char* InputSystem_GetTextInput(void)
{
    return gInputText;
}
