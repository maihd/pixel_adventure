#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <ThirdPartyImpl/imgui_impl_sdl.h>

#include "Native/Input.h"
#include "Native/Window.h"

static WindowDesc* gMainWindow;

static inline KeyCode ConvertKeyCode(int nativeKey)
{
    static bool     s_init = false;
    static KeyCode  s_keyCodeMap[256];
    if (!s_init)
    {
        s_keyCodeMap[SDL_SCANCODE_SPACE]        = KeyCode_Space;
        s_keyCodeMap[SDL_SCANCODE_BACKSPACE]    = KeyCode_Backspace;
        s_keyCodeMap[SDL_SCANCODE_RETURN]       = KeyCode_Return;
        s_keyCodeMap[SDL_SCANCODE_TAB]          = KeyCode_Tab;

        s_keyCodeMap[SDL_SCANCODE_UP]           = KeyCode_UpArrow;
        s_keyCodeMap[SDL_SCANCODE_DOWN]         = KeyCode_DownArrow;
        s_keyCodeMap[SDL_SCANCODE_LEFT]         = KeyCode_LeftArrow;
        s_keyCodeMap[SDL_SCANCODE_RIGHT]        = KeyCode_RightArrow;

        s_keyCodeMap[SDL_SCANCODE_HOME]         = KeyCode_Home;
        s_keyCodeMap[SDL_SCANCODE_END]          = KeyCode_End;
        s_keyCodeMap[SDL_SCANCODE_PAGEUP]       = KeyCode_PageUp;
        s_keyCodeMap[SDL_SCANCODE_PAGEDOWN]     = KeyCode_PageDown;
        s_keyCodeMap[SDL_SCANCODE_INSERT]       = KeyCode_Insert;
        s_keyCodeMap[SDL_SCANCODE_DELETE]       = KeyCode_Delete;

        s_keyCodeMap[SDL_SCANCODE_LCTRL]        = KeyCode_LeftControl;
        s_keyCodeMap[SDL_SCANCODE_RCTRL]        = KeyCode_RightControl;

        s_keyCodeMap[SDL_SCANCODE_LSHIFT]       = KeyCode_LeftShift;
        s_keyCodeMap[SDL_SCANCODE_RSHIFT]       = KeyCode_RightShift;

        s_keyCodeMap[SDL_SCANCODE_LALT]         = KeyCode_LeftAlt;
        s_keyCodeMap[SDL_SCANCODE_RALT]         = KeyCode_RightAlt;

        s_keyCodeMap[SDL_SCANCODE_KP_MULTIPLY]  = KeyCode_NumpadMultiply;
        s_keyCodeMap[SDL_SCANCODE_KP_DIVIDE]    = KeyCode_NumpadDivide;
        s_keyCodeMap[SDL_SCANCODE_KP_PLUS]      = KeyCode_NumpadAdd;
        s_keyCodeMap[SDL_SCANCODE_KP_MINUS]     = KeyCode_NumpadSubtract;
        s_keyCodeMap[SDL_SCANCODE_KP_0]         = KeyCode_Numpad0;
        s_keyCodeMap[SDL_SCANCODE_KP_1]         = KeyCode_Numpad1;
        s_keyCodeMap[SDL_SCANCODE_KP_2]         = KeyCode_Numpad2;
        s_keyCodeMap[SDL_SCANCODE_KP_3]         = KeyCode_Numpad3;
        s_keyCodeMap[SDL_SCANCODE_KP_4]         = KeyCode_Numpad4;
        s_keyCodeMap[SDL_SCANCODE_KP_5]         = KeyCode_Numpad5;
        s_keyCodeMap[SDL_SCANCODE_KP_6]         = KeyCode_Numpad6;
        s_keyCodeMap[SDL_SCANCODE_KP_7]         = KeyCode_Numpad7;
        s_keyCodeMap[SDL_SCANCODE_KP_8]         = KeyCode_Numpad8;
        s_keyCodeMap[SDL_SCANCODE_KP_9]         = KeyCode_Numpad9;

        s_keyCodeMap[SDL_SCANCODE_PERIOD]       = KeyCode_Period;
        s_keyCodeMap[SDL_SCANCODE_MINUS]        = KeyCode_Minus;
        s_keyCodeMap[SDL_SCANCODE_COMMA]        = KeyCode_Comma;
        s_keyCodeMap[SDL_SCANCODE_EQUALS]       = KeyCode_Equal;
        s_keyCodeMap[SDL_SCANCODE_SEMICOLON]    = KeyCode_SemiColon;
        s_keyCodeMap[SDL_SCANCODE_LEFTBRACKET]  = KeyCode_LeftBracket;
        s_keyCodeMap[SDL_SCANCODE_RIGHTBRACKET] = KeyCode_RightBracket;
        s_keyCodeMap[SDL_SCANCODE_SLASH]        = KeyCode_Slash;
        s_keyCodeMap[SDL_SCANCODE_BACKSLASH]    = KeyCode_BackSlash;
        s_keyCodeMap[SDL_SCANCODE_GRAVE]        = KeyCode_BackQuote;

        s_keyCodeMap[SDL_SCANCODE_0]            = KeyCode_Keypad0;
        s_keyCodeMap[SDL_SCANCODE_1]            = KeyCode_Keypad1;
        s_keyCodeMap[SDL_SCANCODE_2]            = KeyCode_Keypad2;
        s_keyCodeMap[SDL_SCANCODE_3]            = KeyCode_Keypad3;
        s_keyCodeMap[SDL_SCANCODE_4]            = KeyCode_Keypad4;
        s_keyCodeMap[SDL_SCANCODE_5]            = KeyCode_Keypad5;
        s_keyCodeMap[SDL_SCANCODE_6]            = KeyCode_Keypad6;
        s_keyCodeMap[SDL_SCANCODE_7]            = KeyCode_Keypad7;
        s_keyCodeMap[SDL_SCANCODE_8]            = KeyCode_Keypad8;
        s_keyCodeMap[SDL_SCANCODE_9]            = KeyCode_Keypad9;

        s_keyCodeMap[SDL_SCANCODE_A]            = KeyCode_A;
        s_keyCodeMap[SDL_SCANCODE_B]            = KeyCode_B;
        s_keyCodeMap[SDL_SCANCODE_C]            = KeyCode_C;
        s_keyCodeMap[SDL_SCANCODE_D]            = KeyCode_D;
        s_keyCodeMap[SDL_SCANCODE_E]            = KeyCode_E;
        s_keyCodeMap[SDL_SCANCODE_G]            = KeyCode_G;
        s_keyCodeMap[SDL_SCANCODE_H]            = KeyCode_H;
        s_keyCodeMap[SDL_SCANCODE_I]            = KeyCode_I;
        s_keyCodeMap[SDL_SCANCODE_J]            = KeyCode_J;
        s_keyCodeMap[SDL_SCANCODE_K]            = KeyCode_K;
        s_keyCodeMap[SDL_SCANCODE_L]            = KeyCode_L;
        s_keyCodeMap[SDL_SCANCODE_M]            = KeyCode_M;
        s_keyCodeMap[SDL_SCANCODE_O]            = KeyCode_O;
        s_keyCodeMap[SDL_SCANCODE_P]            = KeyCode_P;
        s_keyCodeMap[SDL_SCANCODE_R]            = KeyCode_R;
        s_keyCodeMap[SDL_SCANCODE_S]            = KeyCode_S;
        s_keyCodeMap[SDL_SCANCODE_T]            = KeyCode_T;
        s_keyCodeMap[SDL_SCANCODE_U]            = KeyCode_U;
        s_keyCodeMap[SDL_SCANCODE_V]            = KeyCode_V;
        s_keyCodeMap[SDL_SCANCODE_W]            = KeyCode_W;
        s_keyCodeMap[SDL_SCANCODE_X]            = KeyCode_X;
        s_keyCodeMap[SDL_SCANCODE_Y]            = KeyCode_Y;
        s_keyCodeMap[SDL_SCANCODE_Z]            = KeyCode_Z;

        s_keyCodeMap[SDL_SCANCODE_F1]           = KeyCode_F1;
        s_keyCodeMap[SDL_SCANCODE_F2]           = KeyCode_F2;
        s_keyCodeMap[SDL_SCANCODE_F3]           = KeyCode_F3;
        s_keyCodeMap[SDL_SCANCODE_F4]           = KeyCode_F4;
        s_keyCodeMap[SDL_SCANCODE_F5]           = KeyCode_F5;
        s_keyCodeMap[SDL_SCANCODE_F6]           = KeyCode_F6;
        s_keyCodeMap[SDL_SCANCODE_F7]           = KeyCode_F7;
        s_keyCodeMap[SDL_SCANCODE_F8]           = KeyCode_F8;
        s_keyCodeMap[SDL_SCANCODE_F9]           = KeyCode_F9;
        s_keyCodeMap[SDL_SCANCODE_F10]          = KeyCode_F10;
        s_keyCodeMap[SDL_SCANCODE_F11]          = KeyCode_F11;
        s_keyCodeMap[SDL_SCANCODE_F12]          = KeyCode_F12;
        s_keyCodeMap[SDL_SCANCODE_F13]          = KeyCode_F13;
        s_keyCodeMap[SDL_SCANCODE_F14]          = KeyCode_F14;
        s_keyCodeMap[SDL_SCANCODE_F15]          = KeyCode_F15;
        s_keyCodeMap[SDL_SCANCODE_F16]          = KeyCode_F16;
        s_keyCodeMap[SDL_SCANCODE_F17]          = KeyCode_F17;
        s_keyCodeMap[SDL_SCANCODE_F18]          = KeyCode_F18;
        s_keyCodeMap[SDL_SCANCODE_F19]          = KeyCode_F19;
        s_keyCodeMap[SDL_SCANCODE_F20]          = KeyCode_F20;
        s_keyCodeMap[SDL_SCANCODE_F21]          = KeyCode_F21;
        s_keyCodeMap[SDL_SCANCODE_F22]          = KeyCode_F22;
        s_keyCodeMap[SDL_SCANCODE_F23]          = KeyCode_F23;
        s_keyCodeMap[SDL_SCANCODE_F24]          = KeyCode_F24;
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
            Input_UpdateMouse(MouseButton_Left, false);
        }

        if (event->button.button & SDL_BUTTON_RIGHT)
        {
            Input_UpdateMouse(MouseButton_Right, false);
        }

        if (event->button.button & SDL_BUTTON_MIDDLE)
        {
            Input_UpdateMouse(MouseButton_Middle, false);
        }

        if (event->button.button & SDL_BUTTON_X1)
        {
            Input_UpdateMouse(MouseButton_XButton1, false);
        }

        if (event->button.button & SDL_BUTTON_X2)
        {
            Input_UpdateMouse(MouseButton_XButton2, false);
        }
        break;
    }

    case SDL_MOUSEBUTTONUP: {
        if (event->button.button & SDL_BUTTON_LEFT)
        {
            Input_UpdateMouse(MouseButton_Left, true);
        }

        if (event->button.button & SDL_BUTTON_RIGHT)
        {
            Input_UpdateMouse(MouseButton_Right, true);
        }

        if (event->button.button & SDL_BUTTON_MIDDLE)
        {
            Input_UpdateMouse(MouseButton_Middle, true);
        }

        if (event->button.button & SDL_BUTTON_X1)
        {
            Input_UpdateMouse(MouseButton_XButton1, true);
        }

        if (event->button.button & SDL_BUTTON_X2)
        {
            Input_UpdateMouse(MouseButton_XButton2, true);
        }
        break;
    }

    case SDL_MOUSEMOTION:
        Input_UpdateMouseMove((float)event->motion.x, (float)event->motion.y);
        break;

    case SDL_MOUSEWHEEL:
        Input_UpdateMouseWheel((float)event->wheel.x, (float)event->wheel.y);
        break;

    case SDL_KEYUP:
        Input_UpdateKey(ConvertKeyCode((int)event->key.keysym.scancode), false);
        break;

    case SDL_KEYDOWN:
        Input_UpdateKey(ConvertKeyCode((int)event->key.keysym.scancode), true);
        break;

    case SDL_TEXTINPUT:
        Input_UpdateTextInput(event->text.text);
        break;

    case SDL_WINDOWEVENT:
        //Graphics_Viewport(0, 0, Window_GetWidth(), Window_GetHeight());
        break;
    }
}

bool Window_Open(WindowDesc* window)
{
    // Convert to native flags
    uint32_t flags = window->flags;
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

    window->flags &= ~WindowFlags_Quiting; // Prevent game loop suddenly stop 
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
    bool quit = gMainWindow->flags & WindowFlags_Quiting;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (ImGui::GetCurrentContext() != nullptr)
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        if (event.type == SDL_QUIT)
        {
            gMainWindow->flags |= WindowFlags_Quiting;
            quit = true;
        }
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID((SDL_Window*)gMainWindow->handle))
        {
            gMainWindow->flags |= WindowFlags_Quiting;
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

void Window_RequestFocus(void)
{
    if (gMainWindow)
    {
        SDL_SetWindowInputFocus((SDL_Window*)gMainWindow->handle);
    }
}

void Window_MakeGLCurrent(void)
{
    if (gMainWindow)
    {
        SDL_GL_MakeCurrent((SDL_Window*)gMainWindow->handle, gMainWindow->graphicsContext);
    }
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

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
