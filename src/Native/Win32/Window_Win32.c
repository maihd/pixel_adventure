#if defined(_WIN32)
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include "Native/Window.h"
#include "Framework/InputSystem.h"

// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif

#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

static const char* WINDOW_CLASS = "Game Window Class";
static const char* DUMMY_WINDOW = "Game Dummy Window";

static bool             sRegistered = false;
static WNDCLASSEXA      sWindowClass;

static WindowDesc*      gMainWindow;

static KeyCode s_keyCodeMap[2048] = { 
    [VK_SPACE]      = KeyCode_Space,
    [VK_BACK]       = KeyCode_Backspace,
    [VK_RETURN]     = KeyCode_Return,
    [VK_TAB]        = KeyCode_Tab,

    [VK_UP]         = KeyCode_UpArrow,
    [VK_DOWN]       = KeyCode_DownArrow,
    [VK_LEFT]       = KeyCode_LeftArrow,
    [VK_RIGHT]      = KeyCode_RightArrow,

    [VK_HOME]       = KeyCode_Home,
    [VK_END]        = KeyCode_End,
    [VK_PRIOR]      = KeyCode_PageUp,
    [VK_NEXT]       = KeyCode_PageDown,
    [VK_INSERT]     = KeyCode_Insert,
    [VK_DELETE]     = KeyCode_Delete,

    [VK_LCONTROL]   = KeyCode_LeftControl,
    [VK_RCONTROL]   = KeyCode_RightControl,

    [VK_LSHIFT]     = KeyCode_LeftShift,
    [VK_RSHIFT]     = KeyCode_RightShift,

    [VK_MULTIPLY]   = KeyCode_NumpadMultiply,
    [VK_DIVIDE]     = KeyCode_NumpadDivide,
    [VK_ADD]        = KeyCode_NumpadAdd,
    [VK_SUBTRACT]   = KeyCode_NumpadSubtract,
    [VK_NUMPAD0]    = KeyCode_Numpad0,
    [VK_NUMPAD1]    = KeyCode_Numpad1,
    [VK_NUMPAD2]    = KeyCode_Numpad2,
    [VK_NUMPAD3]    = KeyCode_Numpad3,
    [VK_NUMPAD4]    = KeyCode_Numpad4,
    [VK_NUMPAD5]    = KeyCode_Numpad5,
    [VK_NUMPAD6]    = KeyCode_Numpad6,
    [VK_NUMPAD7]    = KeyCode_Numpad7,
    [VK_NUMPAD8]    = KeyCode_Numpad8,
    [VK_NUMPAD9]    = KeyCode_Numpad9,

    [VK_OEM_PERIOD] = KeyCode_Period,
    [VK_OEM_PLUS]   = KeyCode_Plus,
    [VK_OEM_MINUS]  = KeyCode_Minus,
    [VK_OEM_COMMA]  = KeyCode_Comma,
    //s_keyCodeMap[vk_]

    ['0']           = KeyCode_Keypad0,
    ['1']           = KeyCode_Keypad1,
    ['2']           = KeyCode_Keypad2,
    ['3']           = KeyCode_Keypad3,
    ['4']           = KeyCode_Keypad4,
    ['5']           = KeyCode_Keypad5,
    ['6']           = KeyCode_Keypad6,
    ['7']           = KeyCode_Keypad7,
    ['8']           = KeyCode_Keypad8,
    ['9']           = KeyCode_Keypad9,

    ['A']           = KeyCode_A,
    ['B']           = KeyCode_B,
    ['C']           = KeyCode_C,
    ['D']           = KeyCode_D,
    ['E']           = KeyCode_E,
    ['G']           = KeyCode_G,
    ['H']           = KeyCode_H,
    ['I']           = KeyCode_I,
    ['J']           = KeyCode_J,
    ['K']           = KeyCode_K,
    ['L']           = KeyCode_L,
    ['M']           = KeyCode_M,
    ['O']           = KeyCode_O,
    ['P']           = KeyCode_P,
    ['R']           = KeyCode_R,
    ['S']           = KeyCode_S,
    ['T']           = KeyCode_T,
    ['U']           = KeyCode_U,
    ['V']           = KeyCode_V,
    ['W']           = KeyCode_W,
    ['X']           = KeyCode_X,
    ['Y']           = KeyCode_Y,
    ['Z']           = KeyCode_Z,

    ['a']           = KeyCode_A,
    ['b']           = KeyCode_B,
    ['c']           = KeyCode_C,
    ['d']           = KeyCode_D,
    ['e']           = KeyCode_E,
    ['g']           = KeyCode_G,
    ['h']           = KeyCode_H,
    ['i']           = KeyCode_I,
    ['j']           = KeyCode_J,
    ['k']           = KeyCode_K,
    ['l']           = KeyCode_L,
    ['m']           = KeyCode_M,
    ['o']           = KeyCode_O,
    ['p']           = KeyCode_P,
    ['r']           = KeyCode_R,
    ['s']           = KeyCode_S,
    ['t']           = KeyCode_T,
    ['u']           = KeyCode_U,
    ['v']           = KeyCode_V,
    ['w']           = KeyCode_W,
    ['x']           = KeyCode_X,
    ['y']           = KeyCode_Y,
    ['z']           = KeyCode_Z,

    //[','] = KeyCode_Comma;
    //[':'] = KeyCode_Colon;
    [';']           = KeyCode_SemiColon,
    //['*'] = KeyCode_Aterisk;
    //['&'] = KeyCode_Ampersand;
    ['`']           = KeyCode_BackQuote,
    //['~'] = KeyCode_Tilde;
    //['%'] = KeyCode_Percent;
    //['$'] = KeyCode_Dolla;
    //['#'] = KeyCode_Hash;
    //['@'] = KeyCode_At;
    //['!'] = KeyCode_Exclaim;
    //['?'] = KeyCode_Question;
    //['^'] = KeyCode_Caret;
    //['<'] = KeyCode_Less;
    //['='] = KeyCode_Equal;
    //['>'] = KeyCode_Greater;

    ['(']           = KeyCode_LeftParen,
    [')']           = KeyCode_RightParen,
    ['[']           = KeyCode_LeftBracket,
    [']']           = KeyCode_RightBracket,
    ['{']           = KeyCode_LeftCurlyBracket,
    ['}']           = KeyCode_RightCurlyBracket,

    //['\''] = KeyCode_Quote;
    //['\"'] = KeyCode_DoubleQuote;

    ['\/']          = KeyCode_Slash,
    ['\\']          = KeyCode_BackSlash,
};

static inline KeyCode ConvertKeyCode(int nativeKey)
{
    return s_keyCodeMap[nativeKey];
}

static LRESULT CALLBACK Window_Proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Donot handle event from other window
    if (!gMainWindow || hwnd != (HWND)gMainWindow->handle)
    {
        return DefWindowProcA(hwnd, msg, wparam, lparam);;
    }

    switch (msg)
    {
    case WM_LBUTTONUP:
        InputSystem_UpdateMouse(MouseButton_Left, false);
        break;

    case WM_LBUTTONDOWN:
        InputSystem_UpdateMouse(MouseButton_Left, true);
        break;

    case WM_RBUTTONUP:
        InputSystem_UpdateMouse(MouseButton_Right, false);
        break;

    case WM_RBUTTONDOWN:
        InputSystem_UpdateMouse(MouseButton_Right, true);
        break;

    case WM_MBUTTONUP:
        InputSystem_UpdateMouse(MouseButton_Middle, false);
        break;
        
    case WM_MBUTTONDOWN:
        InputSystem_UpdateMouse(MouseButton_Middle, true);
        break;

    case WM_XBUTTONUP:
        InputSystem_UpdateMouse(MouseButton_XButton1, false);
        break;

    case WM_XBUTTONDOWN:
        InputSystem_UpdateMouse(MouseButton_XButton1, true);
        break;

    case WM_MOUSEMOVE:
        InputSystem_UpdateMouseMove((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
        break;

    case WM_MOUSEWHEEL:
        InputSystem_UpdateMouseWheel(0, (float)GET_WHEEL_DELTA_WPARAM(wparam));
        break;

    case WM_MOUSEHWHEEL:
        InputSystem_UpdateMouseWheel((float)GET_WHEEL_DELTA_WPARAM(wparam), 0);
        break;

    case WM_KEYUP:
        InputSystem_UpdateKey(ConvertKeyCode((int)wparam), false);
        break;

    case WM_KEYDOWN:
        InputSystem_UpdateKey(ConvertKeyCode((int)wparam), true);
        break;

    case WM_CHAR:
    case WM_UNICHAR:
        InputSystem_UpdateCharInput((int)wparam);
        break;

    case WM_ERASEBKGND:
    {
        // Make sure to keep consistent background color when resizing.
        HDC hdc = (HDC)wparam;
        RECT rc;
        HBRUSH hbrWhite = CreateSolidBrush(0x00000000);
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, hbrWhite);
        break;
    }

    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_QUIT:
        break;

    case WM_SIZE:
        //Graphics::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());
        break;

    default:
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }

    return 0;
}

static bool Window_RegisterWindowClass(void)
{
    if (!sRegistered)
    {
        memset(&sWindowClass, 0, sizeof(sWindowClass));
        
        sWindowClass.cbSize           = sizeof(sWindowClass);
        sWindowClass.style            = 0;
        sWindowClass.cbClsExtra       = 0;
        sWindowClass.cbWndExtra       = 0;
        sWindowClass.lpfnWndProc      = Window_Proc;
        sWindowClass.hInstance        = (HINSTANCE)GetModuleHandleA(NULL);;
        sWindowClass.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);
        sWindowClass.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
        sWindowClass.hIconSm          = LoadIcon(NULL, IDI_APPLICATION);
        sWindowClass.hCursor          = LoadCursor(NULL, IDC_ARROW);
        sWindowClass.lpszMenuName     = NULL;
        sWindowClass.lpszClassName    = WINDOW_CLASS;
        
        sRegistered = RegisterClassExA(&sWindowClass) != 0;
        
        if (!sRegistered)
        {
            DWORD errorMessageId = GetLastError();
            if (errorMessageId == ERROR_CLASS_ALREADY_EXISTS)
            {
                //AlertDialog::Show("NeonShooter", "Failed to register window");
            }
            else
            {
            }
        }
    }

    return sRegistered;
}

// static void Window_UnregisterWindowClass(void)
// {
//     HINSTANCE hInstance = (HINSTANCE)GetModuleHandleA(NULL);
//     UnregisterClassA(WINDOW_CLASS, hInstance);
//     sRegistered = false;
// }

bool Window_Open(WindowDesc* window)
{
    if (!Window_RegisterWindowClass())
    {
        return false;
    }
    
    // Convert to native flags
    WindowFlags flags = window->flags;
    DWORD nativeFlags = WS_SYSMENU | WS_CAPTION;
    if (flags & WindowFlags_Visible)
    {
        nativeFlags |= WS_VISIBLE;
    }
    if (flags & WindowFlags_Resizable)
    {
        nativeFlags |= WS_OVERLAPPEDWINDOW;
    }

    if (flags & WindowFlags_Minimize)
    {
        nativeFlags |= WS_MINIMIZE;
    }
    if (flags & WindowFlags_Maximize)
    {
        nativeFlags |= WS_MAXIMIZE;
    }
    if (flags & WindowFlags_Minimizable)
    {
        nativeFlags |= WS_MINIMIZEBOX;
    }
    if (flags & WindowFlags_Maximizable)
    {
        nativeFlags |= WS_MAXIMIZEBOX;
    }

    // Calculate actual size of window
    RECT rect;
    rect.left = 0;
    rect.right = window->width;
    rect.top = 0;
    rect.bottom = window->height;
    AdjustWindowRectEx(&rect, nativeFlags, FALSE, 0);

    const int actualWidth = (rect.right - rect.left);
    const int actualHeight = (rect.bottom - rect.top);

    const int actualPosX = (window->x > -1 ? rect.left  + window->x : CW_USEDEFAULT);
    const int actualPosy = (window->y > -1 ? rect.right + window->y : CW_USEDEFAULT);

    // Create native window
    HWND hwnd = CreateWindowExA(
        0,
        WINDOW_CLASS,
        window->title,
        nativeFlags,
        actualPosX, actualPosy, actualWidth, actualHeight,
        NULL, NULL, GetModuleHandleA(NULL), window);
    if (!hwnd)
    {
        return false;
    }

    gMainWindow = window;

    window->handle = (uintptr_t)hwnd;
    window->device = (uintptr_t)GetDC(hwnd);
    
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

    default:
        Window_SetCenter();
        break;
    }

    return true;
}

void Window_Close(WindowDesc* window)
{
    assert(window != NULL);
    assert(gMainWindow == window);

    // Destroyed native handle
    ReleaseDC((HWND)window->handle, (HDC)window->device);
    DestroyWindow((HWND)window->handle);

    // Handle quit message
    Window_PollEvents();

    // Reset window state
    window->handle = 0;
    window->device = 0;

    gMainWindow = NULL;
}

bool Window_PollEvents(void)
{
    bool quit = false;

    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);

        if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
        {
            quit = true;
        }
    }

    return !quit;
}

bool Window_WaitAndPollEvents(void)
{
    MSG msg;
    if (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return false;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
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
        LONG_PTR styles = WS_VISIBLE;
        int      width = GetSystemMetrics(SM_CXSCREEN);
        int      height = GetSystemMetrics(SM_CYSCREEN);

        SetWindowLongPtr((HWND)gMainWindow->handle, GWL_STYLE, styles);
        SetWindowPos((HWND)gMainWindow->handle, HWND_TOP, 0, 0, width, height, 0);
    }
}

void Window_SetFullscreen(void)
{
    if (gMainWindow)
    {
        LONG_PTR styles = WS_POPUP | WS_VISIBLE;
        int      width = GetSystemMetrics(SM_CXSCREEN);
        int      height = GetSystemMetrics(SM_CYSCREEN);

        SetWindowLongPtr((HWND)gMainWindow->handle, GWL_STYLE, styles);
        SetWindowLongPtr((HWND)gMainWindow->handle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
        SetWindowPos((HWND)gMainWindow->handle, HWND_TOPMOST, 0, 0, width, height, 0);
    }
}

bool Window_IsBorderless(void)
{
    return gMainWindow && (gMainWindow->flags & WindowFlags_Borderless);
}

bool Window_IsFullscreen(void)
{
    return gMainWindow && (gMainWindow->flags & WindowFlags_Fullscreen);
}

bool Window_IsWindowed(void)
{
    return !Window_IsBorderless() || !Window_IsFullscreen();
}

void Window_SetWindowed(void)
{
    if (gMainWindow)
    {
        RECT clientRect;
        RECT windowRect;
        GetClientRect((HWND)gMainWindow->handle, &clientRect);
        GetWindowRect((HWND)gMainWindow->handle, &windowRect);

        int moreWidth = (int)((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left));
        int moreHeight = (int)((windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top));

        int width  = gMainWindow->width + moreWidth;
        int height = gMainWindow->height + moreHeight;

        int x, y;
        DWORD flags = 0;
        if (!Window_IsWindowed())
        {
            x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
            y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
            flags = SWP_NOZORDER;
        }
        else
        {
            x = 0;
            y = 0;
            flags = SWP_NOMOVE | SWP_NOZORDER;
        }

        LONG_PTR styles = WS_VISIBLE | WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
        SetWindowLongPtr((HWND)gMainWindow->handle, GWL_STYLE, styles);
        SetWindowPos((HWND)gMainWindow->handle, HWND_TOP, x, y, width, height, flags);
    }
}

bool Window_IsVisible(void)
{
    return IsWindowVisible((HWND)gMainWindow->handle);
}

void Window_SetVisible(bool visible)
{
    if (gMainWindow)
    {
        ShowWindow((HWND)gMainWindow->handle, visible);
    }
}

void Window_SetCenter(void)
{
    if (gMainWindow && Window_IsWindowed())
    {
        int width = gMainWindow->width + GetSystemMetrics(SM_CXBORDER);
        int height = gMainWindow->height + GetSystemMetrics(SM_CYBORDER);

        int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
        int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

        SetWindowPos((HWND)gMainWindow->handle, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
    //if (wglGetSwapIntervalEXT)
    {
        //return wglGetSwapIntervalEXT() != 0;
    }
    //else
    {
        //return false;
    }

    return false;
}

void Window_SetVSyncEnabled(bool vsync)
{
    //if (wglSwapIntervalEXT)
    {
        //wglSwapIntervalEXT((int)vsync);
    }
}

bool Window_HasInputFocus(void)
{
    return (gMainWindow && GetActiveWindow() == (HWND)gMainWindow->handle);
}

bool Window_HasMouseFocus(void)
{
    if (Window_HasInputFocus())
    {
        RECT r;
        POINT p;
        GetCursorPos(&p);
        GetWindowRect((HWND)gMainWindow->handle, &r);

        if (!(p.x < r.left || p.y < r.top || p.x > r.right || p.y > r.bottom))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

const WindowDesc* Window_GetMainWindow(void)
{
    return gMainWindow;
}

#endif //! defined(_WIN32)
