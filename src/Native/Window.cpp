#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "Native/Window.h"
#include "Framework/InputSystem.h"

static WindowDesc* gMainWindow;

static inline KeyCode ConvertKeyCode(int nativeKey)
{
    static bool     s_init = false;
    static KeyCode  s_keyCodeMap[256];
    if (!s_init)
    {
        s_keyCodeMap[SDL_SCANCODE_SPACE]        = KeyCode::Space;
        s_keyCodeMap[SDL_SCANCODE_BACKSPACE]    = KeyCode::Backspace;
        s_keyCodeMap[SDL_SCANCODE_RETURN]       = KeyCode::Return;
        s_keyCodeMap[SDL_SCANCODE_TAB]          = KeyCode::Tab;

        s_keyCodeMap[SDL_SCANCODE_UP]           = KeyCode::UpArrow;
        s_keyCodeMap[SDL_SCANCODE_DOWN]         = KeyCode::DownArrow;
        s_keyCodeMap[SDL_SCANCODE_LEFT]         = KeyCode::LeftArrow;
        s_keyCodeMap[SDL_SCANCODE_RIGHT]        = KeyCode::RightArrow;

        s_keyCodeMap[SDL_SCANCODE_HOME]         = KeyCode::Home;
        s_keyCodeMap[SDL_SCANCODE_END]          = KeyCode::End;
        s_keyCodeMap[SDL_SCANCODE_PAGEUP]       = KeyCode::PageUp;
        s_keyCodeMap[SDL_SCANCODE_PAGEDOWN]     = KeyCode::PageDown;
        s_keyCodeMap[SDL_SCANCODE_INSERT]       = KeyCode::Insert;
        s_keyCodeMap[SDL_SCANCODE_DELETE]       = KeyCode::Delete;

        s_keyCodeMap[SDL_SCANCODE_LCTRL]        = KeyCode::LeftControl;
        s_keyCodeMap[SDL_SCANCODE_RCTRL]        = KeyCode::RightControl;

        s_keyCodeMap[SDL_SCANCODE_LSHIFT]       = KeyCode::LeftShift;
        s_keyCodeMap[SDL_SCANCODE_RSHIFT]       = KeyCode::RightShift;

        s_keyCodeMap[SDL_SCANCODE_LALT]         = KeyCode::LeftAlt;
        s_keyCodeMap[SDL_SCANCODE_RALT]         = KeyCode::RightAlt;

        s_keyCodeMap[SDL_SCANCODE_KP_MULTIPLY]  = KeyCode::NumpadMultiply;
        s_keyCodeMap[SDL_SCANCODE_KP_DIVIDE]    = KeyCode::NumpadDivide;
        s_keyCodeMap[SDL_SCANCODE_KP_PLUS]      = KeyCode::NumpadAdd;
        s_keyCodeMap[SDL_SCANCODE_KP_MINUS]     = KeyCode::NumpadSubtract;
        s_keyCodeMap[SDL_SCANCODE_KP_0]         = KeyCode::Numpad0;
        s_keyCodeMap[SDL_SCANCODE_KP_1]         = KeyCode::Numpad1;
        s_keyCodeMap[SDL_SCANCODE_KP_2]         = KeyCode::Numpad2;
        s_keyCodeMap[SDL_SCANCODE_KP_3]         = KeyCode::Numpad3;
        s_keyCodeMap[SDL_SCANCODE_KP_4]         = KeyCode::Numpad4;
        s_keyCodeMap[SDL_SCANCODE_KP_5]         = KeyCode::Numpad5;
        s_keyCodeMap[SDL_SCANCODE_KP_6]         = KeyCode::Numpad6;
        s_keyCodeMap[SDL_SCANCODE_KP_7]         = KeyCode::Numpad7;
        s_keyCodeMap[SDL_SCANCODE_KP_8]         = KeyCode::Numpad8;
        s_keyCodeMap[SDL_SCANCODE_KP_9]         = KeyCode::Numpad9;

        s_keyCodeMap[SDL_SCANCODE_PERIOD]       = KeyCode::Period;
        s_keyCodeMap[SDL_SCANCODE_MINUS]        = KeyCode::Minus;
        s_keyCodeMap[SDL_SCANCODE_COMMA]        = KeyCode::Comma;
        s_keyCodeMap[SDL_SCANCODE_EQUALS]       = KeyCode::Equal;
        s_keyCodeMap[SDL_SCANCODE_SEMICOLON]    = KeyCode::SemiColon;
        s_keyCodeMap[SDL_SCANCODE_LEFTBRACKET]  = KeyCode::LeftBracket;
        s_keyCodeMap[SDL_SCANCODE_RIGHTBRACKET] = KeyCode::RightBracket;
        s_keyCodeMap[SDL_SCANCODE_SLASH]        = KeyCode::Slash;
        s_keyCodeMap[SDL_SCANCODE_BACKSLASH]    = KeyCode::BackSlash;
        s_keyCodeMap[SDL_SCANCODE_GRAVE]        = KeyCode::BackQuote;

        s_keyCodeMap[SDL_SCANCODE_0]            = KeyCode::Keypad0;
        s_keyCodeMap[SDL_SCANCODE_1]            = KeyCode::Keypad1;
        s_keyCodeMap[SDL_SCANCODE_2]            = KeyCode::Keypad2;
        s_keyCodeMap[SDL_SCANCODE_3]            = KeyCode::Keypad3;
        s_keyCodeMap[SDL_SCANCODE_4]            = KeyCode::Keypad4;
        s_keyCodeMap[SDL_SCANCODE_5]            = KeyCode::Keypad5;
        s_keyCodeMap[SDL_SCANCODE_6]            = KeyCode::Keypad6;
        s_keyCodeMap[SDL_SCANCODE_7]            = KeyCode::Keypad7;
        s_keyCodeMap[SDL_SCANCODE_8]            = KeyCode::Keypad8;
        s_keyCodeMap[SDL_SCANCODE_9]            = KeyCode::Keypad9;

        s_keyCodeMap[SDL_SCANCODE_A]            = KeyCode::A;
        s_keyCodeMap[SDL_SCANCODE_B]            = KeyCode::B;
        s_keyCodeMap[SDL_SCANCODE_C]            = KeyCode::C;
        s_keyCodeMap[SDL_SCANCODE_D]            = KeyCode::D;
        s_keyCodeMap[SDL_SCANCODE_E]            = KeyCode::E;
        s_keyCodeMap[SDL_SCANCODE_G]            = KeyCode::G;
        s_keyCodeMap[SDL_SCANCODE_H]            = KeyCode::H;
        s_keyCodeMap[SDL_SCANCODE_I]            = KeyCode::I;
        s_keyCodeMap[SDL_SCANCODE_J]            = KeyCode::J;
        s_keyCodeMap[SDL_SCANCODE_K]            = KeyCode::K;
        s_keyCodeMap[SDL_SCANCODE_L]            = KeyCode::L;
        s_keyCodeMap[SDL_SCANCODE_M]            = KeyCode::M;
        s_keyCodeMap[SDL_SCANCODE_O]            = KeyCode::O;
        s_keyCodeMap[SDL_SCANCODE_P]            = KeyCode::P;
        s_keyCodeMap[SDL_SCANCODE_R]            = KeyCode::R;
        s_keyCodeMap[SDL_SCANCODE_S]            = KeyCode::S;
        s_keyCodeMap[SDL_SCANCODE_T]            = KeyCode::T;
        s_keyCodeMap[SDL_SCANCODE_U]            = KeyCode::U;
        s_keyCodeMap[SDL_SCANCODE_V]            = KeyCode::V;
        s_keyCodeMap[SDL_SCANCODE_W]            = KeyCode::W;
        s_keyCodeMap[SDL_SCANCODE_X]            = KeyCode::X;
        s_keyCodeMap[SDL_SCANCODE_Y]            = KeyCode::Y;
        s_keyCodeMap[SDL_SCANCODE_Z]            = KeyCode::Z;

        s_keyCodeMap[SDL_SCANCODE_F1]           = KeyCode::F1;
        s_keyCodeMap[SDL_SCANCODE_F2]           = KeyCode::F2;
        s_keyCodeMap[SDL_SCANCODE_F3]           = KeyCode::F3;
        s_keyCodeMap[SDL_SCANCODE_F4]           = KeyCode::F4;
        s_keyCodeMap[SDL_SCANCODE_F5]           = KeyCode::F5;
        s_keyCodeMap[SDL_SCANCODE_F6]           = KeyCode::F6;
        s_keyCodeMap[SDL_SCANCODE_F7]           = KeyCode::F7;
        s_keyCodeMap[SDL_SCANCODE_F8]           = KeyCode::F8;
        s_keyCodeMap[SDL_SCANCODE_F9]           = KeyCode::F9;
        s_keyCodeMap[SDL_SCANCODE_F10]          = KeyCode::F10;
        s_keyCodeMap[SDL_SCANCODE_F11]          = KeyCode::F11;
        s_keyCodeMap[SDL_SCANCODE_F12]          = KeyCode::F12;
        s_keyCodeMap[SDL_SCANCODE_F13]          = KeyCode::F13;
        s_keyCodeMap[SDL_SCANCODE_F14]          = KeyCode::F14;
        s_keyCodeMap[SDL_SCANCODE_F15]          = KeyCode::F15;
        s_keyCodeMap[SDL_SCANCODE_F16]          = KeyCode::F16;
        s_keyCodeMap[SDL_SCANCODE_F17]          = KeyCode::F17;
        s_keyCodeMap[SDL_SCANCODE_F18]          = KeyCode::F18;
        s_keyCodeMap[SDL_SCANCODE_F19]          = KeyCode::F19;
        s_keyCodeMap[SDL_SCANCODE_F20]          = KeyCode::F20;
        s_keyCodeMap[SDL_SCANCODE_F21]          = KeyCode::F21;
        s_keyCodeMap[SDL_SCANCODE_F22]          = KeyCode::F22;
        s_keyCodeMap[SDL_SCANCODE_F23]          = KeyCode::F23;
        s_keyCodeMap[SDL_SCANCODE_F24]          = KeyCode::F24;
    }

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
    WindowFlags flags = (WindowFlags)window->flags;
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

    window->handle = handle;
    gMainWindow = window;
    
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
    SDL_DestroyWindow((SDL_Window*)window->handle);

    // Handle quit message
    Window_PollEvents();

    // Reset window state
    window->handle = nullptr;
    gMainWindow = nullptr;
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
        SDL_SetWindowFullscreen((SDL_Window*)gMainWindow->handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
}

void Window_SetFullscreen(void)
{
    if (gMainWindow)
    {
        gMainWindow->flags &= ~WindowFlags_Borderless;
        gMainWindow->flags |=  WindowFlags_Fullscreen;
        SDL_SetWindowFullscreen((SDL_Window*)gMainWindow->handle, SDL_WINDOW_FULLSCREEN);
    }
}

bool Window_IsBorderless(void)
{
    return gMainWindow && SDL_GetWindowFlags((SDL_Window*)gMainWindow->handle) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

bool Window_IsFullscreen(void)
{
    return gMainWindow && SDL_GetWindowFlags((SDL_Window*)gMainWindow->handle) & SDL_WINDOW_FULLSCREEN;
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
        SDL_SetWindowFullscreen((SDL_Window*)gMainWindow->handle, 0);
        SDL_SetWindowSize((SDL_Window*)gMainWindow->handle, gMainWindow->width, gMainWindow->height);
    }
}

bool Window_IsVisible(void)
{
    return gMainWindow && SDL_GetWindowFlags((SDL_Window*)gMainWindow->handle) & SDL_WINDOW_SHOWN;
}

void Window_SetVisible(bool visible)
{
    if (gMainWindow)
    {
        if (visible)
        {
            gMainWindow->flags |= WindowFlags_Visible;
            SDL_ShowWindow((SDL_Window*)gMainWindow->handle);
        }
        else
        {
            gMainWindow->flags &= ~WindowFlags_Visible;
            SDL_ShowWindow((SDL_Window*)gMainWindow->handle);
        }
    }
}

void Window_SetCenter(void)
{
    if (gMainWindow && Window_IsWindowed())
    {
        SDL_SetWindowPosition((SDL_Window*)gMainWindow->handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
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
