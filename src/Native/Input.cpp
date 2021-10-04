#include <string.h>
#include <SDL2/SDL.h>

#include "Input.h"

#define COUNTOF(x) (sizeof(x) / sizeof((x)[0]))

constexpr int32_t KEY_INPUT_COUNT       = 256;
constexpr int32_t MOUSE_INPUT_COUNT     = 8;

constexpr int32_t GAMEPAD_AXIS_COUNT    = 8;
constexpr int32_t GAMEPAD_BUTTON_COUNT  = 32;
constexpr int32_t MAX_GAME_CONTROLLERS  = 8;

typedef struct GamepadState
{
    float       axis[GAMEPAD_AXIS_COUNT];
    bool        buttonPressed[GAMEPAD_BUTTON_COUNT];
    int32_t     buttonUpFrame[GAMEPAD_BUTTON_COUNT];
    int32_t     buttonDownFrame[GAMEPAD_BUTTON_COUNT];
} GamepadState;

static int32_t              gCurrentFrame;

static int32_t              gInputTextLength;
static char                 gInputText[1024];

static bool                 gKeyState[KEY_INPUT_COUNT];
static int32_t              gKeyUpFrame[KEY_INPUT_COUNT];
static int32_t              gKeyDownFrame[KEY_INPUT_COUNT];

static int32_t              gMouseState;
static int32_t              gMouseUpFrame[MOUSE_INPUT_COUNT];
static int32_t              gMouseDownFrame[MOUSE_INPUT_COUNT];
static float                gMouseX;
static float                gMouseY;

static float                gMouseWheelH;
static float                gMouseWheelV;

static GamepadState         gGamepadStates[MAX_GAME_CONTROLLERS];
static SDL_GameController*  gGameControllers[MAX_GAME_CONTROLLERS];

static void OpenGameControllers()
{
    int index = 0;
    for (int i = 0, n = MAX_GAME_CONTROLLERS; i < n; i++)
    {
        if (SDL_IsGameController(i))
        {
            gGameControllers[index++] = SDL_GameControllerOpen(i);
        }
    }

    while (index < MAX_GAME_CONTROLLERS)
    {
        gGameControllers[index++] = nullptr;
    }
}

static void CloseGameControllers()
{
    for (int i = 0, n = MAX_GAME_CONTROLLERS; i < n; i++)
    {
        if (gGameControllers[i])
        {
            SDL_GameControllerClose(gGameControllers[i]);
            gGameControllers[i] = nullptr;
        }
    }
}

static float GetJoystickAxis(SDL_GameController* gameController, SDL_GameControllerAxis axis)
{
    constexpr float FACTOR = 1.0f / ((float)SDL_MAX_SINT16);
    constexpr int   DEAD_ZONE = 4000;

    int value = SDL_GameControllerGetAxis(gameController, axis);

    if (value > DEAD_ZONE)
    {
        return value * FACTOR;
    }
    else if (value < -DEAD_ZONE)
    {
        return value * FACTOR;
    }
    else
    {
        return 0.0f;
    }
}

void Input::Setup(void)
{
    gCurrentFrame = 1;
    
    // Find game controller
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) == 0)
    {
        OpenGameControllers();
    }
}

void Input::Shutdown(void)
{
    CloseGameControllers();
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
}

void Input::NewFrame(void)
{
    gInputTextLength = 0;
    OpenGameControllers();
}

void Input::EndFrame(void)
{
    gInputText[gInputTextLength] = 0;
    gCurrentFrame++;

    SDL_GameControllerUpdate();
    for (int i = 0, n = MAX_GAME_CONTROLLERS; i < n; i++)
    {
        SDL_GameController* gameController = gGameControllers[i];
        if (SDL_GameControllerGetAttached(gameController))
        {
            float leftVertical = -GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);
            float leftHorizontal = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);

            float rightVertical = -GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY);
            float rightHorizontal = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX);

            float leftTrigger = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            float rightTrigger = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

            GamepadState& gamepadState = gGamepadStates[i];
            gamepadState.axis[(int)GamepadAxis::LeftVertical] = leftVertical;
            gamepadState.axis[(int)GamepadAxis::LeftHorizontal] = leftHorizontal;

            gamepadState.axis[(int)GamepadAxis::RightVertical] = rightVertical;
            gamepadState.axis[(int)GamepadAxis::RightHorizontal] = rightHorizontal;

            gamepadState.axis[(int)GamepadAxis::LeftTrigger] = leftTrigger;
            gamepadState.axis[(int)GamepadAxis::RightTrigger] = rightTrigger;

            for (int button = 0; button < GAMEPAD_BUTTON_COUNT; button++)
            {
                bool newState = SDL_GameControllerGetButton(gameController, (SDL_GameControllerButton)button);
                bool oldState = gamepadState.buttonPressed[button];

                if (oldState != newState)
                {
                    gamepadState.buttonPressed[button] = newState;
                    if (newState)
                    {
                        gamepadState.buttonDownFrame[button] = gCurrentFrame;
                    }
                    else
                    {
                        gamepadState.buttonUpFrame[button] = gCurrentFrame;
                    }
                }
            }
        }
    }
}

void Input::UpdateCharInput(int character)
{
    gInputText[gInputTextLength++] = (char)character;
}

void Input::UpdateTextInput(const char* string)
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

void Input::UpdateKey(KeyCode key, bool down)
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

void Input::UpdateMouse(MouseButton button, bool down)
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

void Input::UpdateMouseMove(float x, float y)
{
    gMouseX = x;
    gMouseY = y;
}

void Input::UpdateMouseWheel(float h, float v)
{
    gMouseWheelV += v;
    gMouseWheelH += h;
}

void Input::UpdateGamepadAxis(int gamepadId, GamepadAxis axis, float value)
{
    if (gamepadId > -1 && gamepadId < COUNTOF(gGamepadStates))
    {
        GamepadState* state = &gGamepadStates[gamepadId];

        int index = (int)axis;
        if (index > -1 && index < COUNTOF(state->axis))
        {
            state->axis[index] = value;
        }
    }
}

void Input::UpdateGamepadButton(int gamepadId, GamepadButton button, bool down)
{
    if (gamepadId > -1 && gamepadId < COUNTOF(gGamepadStates))
    {
        GamepadState* state = &gGamepadStates[gamepadId];

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

void Input::ShowMouseCursor(void)
{
    Input::SetMouseCursorVisible(true);
}

void Input::HideMouseCursor(void)
{
    Input::SetMouseCursorVisible(false);
}

bool Input::IsMouseCursorVisibled(void)
{
    return false;
}

void Input::SetMouseCursorVisible(bool visible)
{

}

int Input::GetMouseState(float* x, float* y)
{
    if (x) *x = gMouseX;
    if (y) *y = gMouseY;

    return gMouseState;
}

bool Input::GetMouseButton(MouseButton button)
{
    int index = (int)button;
    if (index > -1 && index < COUNTOF(gMouseDownFrame) && index < COUNTOF(gMouseUpFrame))
    {
        return gMouseDownFrame[index] > gMouseUpFrame[index];
    }

    return false;
}

bool Input::GetMouseButtonUp(MouseButton button)
{
    int index = (int)button;
    if (index > -1 && index < COUNTOF(gMouseUpFrame))
    {
        return gMouseUpFrame[index] == gCurrentFrame;
    }

    return false;
}

bool Input::GetMouseButtonDown(MouseButton button)
{
    int index = (int)button;
    if (index > -1 && index < COUNTOF(gMouseDownFrame))
    {
        return gMouseDownFrame[index] == gCurrentFrame;
    }

    return false;
}

bool Input::GetKey(KeyCode key)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        return gKeyState[index];
    }

    return false;
}

bool Input::GetKeyUp(KeyCode key)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        return gKeyUpFrame[index] == gCurrentFrame;
    }

    return false;
}

bool Input::GetKeyDown(KeyCode key)
{
    int index = (int)key;
    if (index > -1 && index < KEY_INPUT_COUNT)
    {
        return gKeyDownFrame[index] == gCurrentFrame;
    }

    return false;
}

bool Input::IsGamepadAttached(int32_t gamepadId)
{
    if (gamepadId > -1 && gamepadId < MAX_GAME_CONTROLLERS)
    {
        return SDL_GameControllerGetAttached(gGameControllers[gamepadId]);
    }
    else
    {
        return false;
    }
}

float Input::GetAxis(int32_t gamepadId, GamepadAxis axis)
{
    if (gamepadId > -1 && gamepadId < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadId];

        int index = (int)axis;
        if (index > -1 && index < COUNTOF(state->axis))
        {
            return state->axis[index];
        }
    }

    return 0.0f;
}

bool Input::GetButton(int32_t gamepadId, GamepadButton button)
{
    if (gamepadId > -1 && gamepadId < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadId];

        int index = (int)button;
        if (index > -1 && index < COUNTOF(state->buttonDownFrame) && index < COUNTOF(state->buttonUpFrame))
        {
            return state->buttonDownFrame[index] > state->buttonUpFrame[index];
        }
    }

    return false;
}

bool Input::GetButtonUp(int32_t gamepadId, GamepadButton button)
{
    if (gamepadId > -1 && gamepadId < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadId];

        int index = (int)button;
        if (index > -1 && index < COUNTOF(state->buttonUpFrame))
        {
            return state->buttonUpFrame[index] == gCurrentFrame;
        }
    }

    return false;
}

bool Input::GetButtonDown(int32_t gamepadId, GamepadButton button)
{
    if (gamepadId > -1 && gamepadId < COUNTOF(gGamepadStates))
    {
        const GamepadState* state = &gGamepadStates[gamepadId];

        int index = (int)button;
        if (index > -1 && index < COUNTOF(state->buttonDownFrame))
        {
            return state->buttonDownFrame[index] == gCurrentFrame;
        }
    }

    return false;
}

const char* Input::GetTextInput(void)
{
    return gInputText;
}
