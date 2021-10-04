#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "Native/Window.h"
#include "Framework/InputSystem.h"

static WindowDesc*      gMainWindow;

static KeyCode s_keyCodeMap[256] = { 
    [SDL_SCANCODE_SPACE]        = KeyCode_Space,
    [SDL_SCANCODE_BACKSPACE]    = KeyCode_Backspace,
    [SDL_SCANCODE_RETURN]       = KeyCode_Return,
    [SDL_SCANCODE_TAB]          = KeyCode_Tab,

    [SDL_SCANCODE_UP]           = KeyCode_UpArrow,
    [SDL_SCANCODE_DOWN]         = KeyCode_DownArrow,
    [SDL_SCANCODE_LEFT]         = KeyCode_LeftArrow,
    [SDL_SCANCODE_RIGHT]        = KeyCode_RightArrow,

    [SDL_SCANCODE_HOME]         = KeyCode_Home,
    [SDL_SCANCODE_END]          = KeyCode_End,
    [SDL_SCANCODE_PAGEUP]       = KeyCode_PageUp,
    [SDL_SCANCODE_PAGEDOWN]     = KeyCode_PageDown,
    [SDL_SCANCODE_INSERT]       = KeyCode_Insert,
    [SDL_SCANCODE_DELETE]       = KeyCode_Delete,

    [SDL_SCANCODE_LCTRL]        = KeyCode_LeftControl,
    [SDL_SCANCODE_RCTRL]        = KeyCode_RightControl,

    [SDL_SCANCODE_LSHIFT]       = KeyCode_LeftShift,
    [SDL_SCANCODE_RSHIFT]       = KeyCode_RightShift,

    [SDL_SCANCODE_LALT]         = KeyCode_LeftAlt,
    [SDL_SCANCODE_RALT]         = KeyCode_RightAlt,

    [SDL_SCANCODE_KP_MULTIPLY]  = KeyCode_NumpadMultiply,
    [SDL_SCANCODE_KP_DIVIDE]    = KeyCode_NumpadDivide,
    [SDL_SCANCODE_KP_PLUS]      = KeyCode_NumpadAdd,
    [SDL_SCANCODE_KP_MINUS]     = KeyCode_NumpadSubtract,
    [SDL_SCANCODE_KP_0]         = KeyCode_Numpad0,
    [SDL_SCANCODE_KP_1]         = KeyCode_Numpad1,
    [SDL_SCANCODE_KP_2]         = KeyCode_Numpad2,
    [SDL_SCANCODE_KP_3]         = KeyCode_Numpad3,
    [SDL_SCANCODE_KP_4]         = KeyCode_Numpad4,
    [SDL_SCANCODE_KP_5]         = KeyCode_Numpad5,
    [SDL_SCANCODE_KP_6]         = KeyCode_Numpad6,
    [SDL_SCANCODE_KP_7]         = KeyCode_Numpad7,
    [SDL_SCANCODE_KP_8]         = KeyCode_Numpad8,
    [SDL_SCANCODE_KP_9]         = KeyCode_Numpad9,

    [SDL_SCANCODE_PERIOD]       = KeyCode_Period,
    [SDL_SCANCODE_MINUS]        = KeyCode_Minus,
    [SDL_SCANCODE_COMMA]        = KeyCode_Comma,
    [SDL_SCANCODE_EQUALS]       = KeyCode_Equal,
    [SDL_SCANCODE_SEMICOLON]    = KeyCode_SemiColon,
    [SDL_SCANCODE_LEFTBRACKET]  = KeyCode_LeftBracket,
    [SDL_SCANCODE_RIGHTBRACKET] = KeyCode_RightBracket,
    [SDL_SCANCODE_SLASH]        = KeyCode_Slash,
    [SDL_SCANCODE_BACKSLASH]    = KeyCode_BackSlash,
    [SDL_SCANCODE_GRAVE]        = KeyCode_BackQuote,

    [SDL_SCANCODE_0]           = KeyCode_Keypad0,
    [SDL_SCANCODE_1]           = KeyCode_Keypad1,
    [SDL_SCANCODE_2]           = KeyCode_Keypad2,
    [SDL_SCANCODE_3]           = KeyCode_Keypad3,
    [SDL_SCANCODE_4]           = KeyCode_Keypad4,
    [SDL_SCANCODE_5]           = KeyCode_Keypad5,
    [SDL_SCANCODE_6]           = KeyCode_Keypad6,
    [SDL_SCANCODE_7]           = KeyCode_Keypad7,
    [SDL_SCANCODE_8]           = KeyCode_Keypad8,
    [SDL_SCANCODE_9]           = KeyCode_Keypad9,

    [SDL_SCANCODE_A]           = KeyCode_A,
    [SDL_SCANCODE_B]           = KeyCode_B,
    [SDL_SCANCODE_C]           = KeyCode_C,
    [SDL_SCANCODE_D]           = KeyCode_D,
    [SDL_SCANCODE_E]           = KeyCode_E,
    [SDL_SCANCODE_G]           = KeyCode_G,
    [SDL_SCANCODE_H]           = KeyCode_H,
    [SDL_SCANCODE_I]           = KeyCode_I,
    [SDL_SCANCODE_J]           = KeyCode_J,
    [SDL_SCANCODE_K]           = KeyCode_K,
    [SDL_SCANCODE_L]           = KeyCode_L,
    [SDL_SCANCODE_M]           = KeyCode_M,
    [SDL_SCANCODE_O]           = KeyCode_O,
    [SDL_SCANCODE_P]           = KeyCode_P,
    [SDL_SCANCODE_R]           = KeyCode_R,
    [SDL_SCANCODE_S]           = KeyCode_S,
    [SDL_SCANCODE_T]           = KeyCode_T,
    [SDL_SCANCODE_U]           = KeyCode_U,
    [SDL_SCANCODE_V]           = KeyCode_V,
    [SDL_SCANCODE_W]           = KeyCode_W,
    [SDL_SCANCODE_X]           = KeyCode_X,
    [SDL_SCANCODE_Y]           = KeyCode_Y,
    [SDL_SCANCODE_Z]           = KeyCode_Z,
};

static inline KeyCode ConvertKeyCode(int nativeKey)
{
    return s_keyCodeMap[nativeKey];
}

static void HandleEvent(const SDL_Event* event)
{
    // Donot handle event from other window
    if (!gMainWindow)
    {
        return;
    }

    switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN: {
        if (event->button.button & SDL_BUTTON_LEFT)
        {
            InputSystem_UpdateMouse(MouseButton_Left, false);
        }

        if (event->button.button & SDL_BUTTON_RIGHT)
        {
            InputSystem_UpdateMouse(MouseButton_Right, false);
        }

        if (event->button.button & SDL_BUTTON_MIDDLE)
        {
            InputSystem_UpdateMouse(MouseButton_Middle, false);
        }

        if (event->button.button & SDL_BUTTON_X1)
        {
            InputSystem_UpdateMouse(MouseButton_XButton1, false);
        }

        if (event->button.button & SDL_BUTTON_X2)
        {
            InputSystem_UpdateMouse(MouseButton_XButton2, false);
        }
        break;
    }

    case SDL_MOUSEBUTTONUP: {
        if (event->button.button & SDL_BUTTON_LEFT)
        {
            InputSystem_UpdateMouse(MouseButton_Left, true);
        }

        if (event->button.button & SDL_BUTTON_RIGHT)
        {
            InputSystem_UpdateMouse(MouseButton_Right, true);
        }

        if (event->button.button & SDL_BUTTON_MIDDLE)
        {
            InputSystem_UpdateMouse(MouseButton_Middle, true);
        }

        if (event->button.button & SDL_BUTTON_X1)
        {
            InputSystem_UpdateMouse(MouseButton_XButton1, true);
        }

        if (event->button.button & SDL_BUTTON_X2)
        {
            InputSystem_UpdateMouse(MouseButton_XButton2, true);
        }
        break;
    }

    case SDL_MOUSEMOTION:
        InputSystem_UpdateMouseMove((float)event->motion.x, (float)event->motion.y);
        break;

    case SDL_MOUSEWHEEL:
        InputSystem_UpdateMouseWheel((float)event->wheel.x, (float)event->wheel.y);
        break;

    case SDL_KEYUP:
        InputSystem_UpdateKey(ConvertKeyCode((int)event->key.keysym.scancode), false);
        break;

    case SDL_KEYDOWN:
        InputSystem_UpdateKey(ConvertKeyCode((int)event->key.keysym.scancode), true);
        break;

    case SDL_TEXTINPUT:
        InputSystem_UpdateTextInput(event->text.text);
        break;

    case SDL_WINDOWEVENT:
        //Graphics::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());
        break;
    }
}

bool Window_Open(WindowDesc* window)
{
    // Convert to native flags
    WindowFlags flags = window->flags;
    Uint32 nativeFlags = SDL_WINDOW_OPENGL;
    if (flags & WindowFlags_Visible)
    {
        nativeFlags |= SDL_WINDOW_SHOWN;
    }
    if (flags & WindowFlags_Resizable)
    {
        nativeFlags |= SDL_WINDOW_RESIZABLE;
    }

    if (flags & WindowFlags_Minimize)
    {
        nativeFlags |= SDL_WINDOW_MINIMIZED;
    }
    if (flags & WindowFlags_Maximize)
    {
        nativeFlags |= SDL_WINDOW_MAXIMIZED;
    }
    if (flags & WindowFlags_Minimizable)
    {
        //nativeFlags |= SDL_WINDOW_;
    }
    if (flags & WindowFlags_Maximizable)
    {
        //nativeFlags |= WS_MAXIMIZEBOX;
    }

    // Calculate actual size of window
    const int actualWidth = window->width;
    const int actualHeight = window->height;

    const int actualPosX = (window->x > -1 ? window->x : SDL_WINDOWPOS_CENTERED);
    const int actualPosy = (window->y > -1 ? window->y : SDL_WINDOWPOS_CENTERED);

    // Create native window
    SDL_Window* handle = SDL_CreateWindow(window->title, actualPosX, actualPosy, actualWidth, actualHeight, nativeFlags);
    if (!handle)
    {
        return false;
    }

    // Create OpenGL Context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GLContext context = SDL_GL_CreateContext(handle);
    if (!context)
    {
        SDL_DestroyWindow(handle);
        return false;
    }

    gMainWindow = window;

    window->handle = handle;
    window->context = context;
    
    // Apply borderless or fullscreen
    switch (flags & (WindowFlags_Borderless | WindowFlags_Fullscreen))
    {
    case WindowFlags_Borderless:
    case WindowFlags_Borderless | WindowFlags_Fullscreen:
        Window_SetBorderless();
        break;

    case WindowFlags_Fullscreen:
        Window_SetFullscreen();
        break;
    }

    return true;
}

void Window_Close(WindowDesc* window)
{
    assert(window != NULL);
    assert(gMainWindow == window);

    // Destroyed native handle
    SDL_GL_DeleteContext(window->context);
    SDL_DestroyWindow(window->handle);

    // Handle quit message
    Window_PollEvents();

    // Reset window state
    window->handle = NULL;
    window->context = NULL;

    gMainWindow = NULL;
}

bool Window_PollEvents(void)
{
    bool quit = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else
        {
            HandleEvent(&event);
        }
    }

    return !quit;
}

bool Window_WaitAndPollEvents(void)
{
    SDL_Event event;
    if (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
        else
        {
            HandleEvent(&event);
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return false;
            }
            else
            {
                HandleEvent(&event);
            }
        }
    }

    return true;
}

float Window_GetWidth(void)
{
    return (float)gMainWindow->width;
}

float Window_GetHeight(void)
{
    return (float)gMainWindow->height;
}

void Window_SetBorderless(void)
{
    if (gMainWindow)
    {
        gMainWindow->flags |=  WindowFlags_Borderless;
        gMainWindow->flags &= ~WindowFlags_Fullscreen;
        SDL_SetWindowFullscreen(gMainWindow->handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
}

void Window_SetFullscreen(void)
{
    if (gMainWindow)
    {
        gMainWindow->flags &= ~WindowFlags_Borderless;
        gMainWindow->flags |=  WindowFlags_Fullscreen;
        SDL_SetWindowFullscreen(gMainWindow->handle, SDL_WINDOW_FULLSCREEN);
    }
}

bool Window_IsBorderless(void)
{
    return gMainWindow && SDL_GetWindowFlags(gMainWindow->handle) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

bool Window_IsFullscreen(void)
{
    return gMainWindow && SDL_GetWindowFlags(gMainWindow->handle) & SDL_WINDOW_FULLSCREEN;
}

bool Window_IsWindowed(void)
{
    return !Window_IsBorderless() || !Window_IsFullscreen();
}

void Window_SetWindowed(void)
{
    if (gMainWindow)
    {
        gMainWindow->flags &= ~WindowFlags_Borderless;
        gMainWindow->flags &= ~WindowFlags_Fullscreen;
        SDL_SetWindowFullscreen(gMainWindow->handle, 0);
        SDL_SetWindowSize(gMainWindow->handle, gMainWindow->width, gMainWindow->height);
    }
}

bool Window_IsVisible(void)
{
    return gMainWindow && SDL_GetWindowFlags(gMainWindow->handle) & SDL_WINDOW_SHOWN;
}

void Window_SetVisible(bool visible)
{
    if (gMainWindow)
    {
        if (visible)
        {
            gMainWindow->flags |= WindowFlags_Visible;
            SDL_ShowWindow(gMainWindow->handle);
        }
        else
        {
            gMainWindow->flags &= ~WindowFlags_Visible;
            SDL_ShowWindow(gMainWindow->handle);
        }
    }
}

void Window_SetCenter(void)
{
    if (gMainWindow && Window_IsWindowed())
    {
        SDL_SetWindowPosition(gMainWindow->handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

void Window_EnableVSync(void)
{
    Window_SetVSyncEnabled(true);
}

void Window_DisableVSync(void)
{
    Window_SetVSyncEnabled(false);
}

bool Window_IsVSyncEnabled(void)
{
    return SDL_GL_GetSwapInterval();
}

void Window_SetVSyncEnabled(bool vsync)
{
    SDL_GL_SetSwapInterval(vsync);
}

bool Window_HasInputFocus(void)
{
    return gMainWindow && SDL_GetKeyboardFocus() == gMainWindow->handle;
}

bool Window_HasMouseFocus(void)
{
    return gMainWindow && SDL_GetMouseFocus() == gMainWindow->handle;
}

const WindowDesc* Window_GetMainWindow(void)
{
    return gMainWindow;
}
