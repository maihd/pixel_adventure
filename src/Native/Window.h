#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <assert.h> // For auto unclosed window attempt to be destroyed
#endif

typedef enum WindowFlags
{
    WindowFlags_None          = 0,
    WindowFlags_Visible       = 1 << 0,
    WindowFlags_Resizable     = 1 << 1,
    WindowFlags_Fullscreen    = 1 << 2,
    WindowFlags_Borderless    = 1 << 3,

    WindowFlags_Minimize      = 1 << 5,
    WindowFlags_Maximize      = 1 << 6,
    WindowFlags_Minimizable   = 1 << 7,
    WindowFlags_Maximizable   = 1 << 8,

    WindowFlags_Default       = WindowFlags_Visible | WindowFlags_Minimizable,
} WindowFlags;

typedef enum WindowResetScenario
{
    WindowResetScenario_None        = 0x0,
    WindowResetScenario_Reload      = 0x1,
    WindowResetScenario_DeviceLost  = 0x2,
} WindowResetScenario;

typedef struct WindowDesc
{
    void*               handle;

    uint32_t            flags;
    const char*         title;

    int32_t             x;
    int32_t             y;
    int32_t             width;
    int32_t             height;

    WindowResetScenario resetScenario;

    #ifdef __cplusplus
    inline WindowDesc()
        : handle(nullptr)
        , flags(WindowFlags_None)
        , title("")
        , width(0)
        , height(0)
        , x(-1)
        , y(-1)
        , resetScenario(WindowResetScenario_None)
    {
    }

    inline ~WindowDesc()
    {
        assert(handle == nullptr);
    }
    #endif
} WindowDesc;

// -------------------------------------------------------------
// Main functions
// -------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

bool                Window_Open(WindowDesc* window);
void                Window_Close(WindowDesc* window);

/// Process all coming events
/// Return true if the window is alive, false otherwise, and the loop should shutdown
bool                Window_PollEvents(void); 

/// Wait a events and then process all coming events
/// Return true if the window is alive, false otherwise, and the loop should shutdown
bool                Window_WaitAndPollEvents(void);

float               Window_GetWidth(void);
float               Window_GetHeight(void);

bool                Window_IsBorderless(void);
bool                Window_IsFullscreen(void);
//bool              Window_IsFullscreenSettings(void);

void                Window_SetBorderless(void);
void                Window_SetFullscreen(void);

bool                Window_IsWindowed(void);
void                Window_SetWindowed(void);

bool                Window_IsVisible(void);
void                Window_SetVisible(bool visible);

void                Window_SetCenter(void);

void                Window_EnableVSync(void);
void                Window_DisableVSync(void);
bool                Window_IsVSyncEnabled(void);
void                Window_SetVSyncEnabled(bool value);

bool                Window_HasInputFocus(void);
bool                Window_HasMouseFocus(void);

const WindowDesc*   Window_GetMainWindow(void);

#ifdef __cplusplus
}
#endif
