#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "Misc/Compiler.h"

typedef uint32_t WindowFlags;
typedef uint32_t WindowResetScenario;

enum WindowFlags_ __enum_type(uint32_t)
{
    WindowFlags_None            = 0,
    WindowFlags_Visible         = 1 << 0,
    WindowFlags_Resizable       = 1 << 1,
    WindowFlags_Fullscreen      = 1 << 2,
    WindowFlags_Borderless      = 1 << 3,

    WindowFlags_Minimize        = 1 << 5,
    WindowFlags_Maximize        = 1 << 6,
    WindowFlags_Minimizable     = 1 << 7,
    WindowFlags_Maximizable     = 1 << 8,

    WindowFlags_Quiting         = 1 << 9,

    WindowFlags_Default         = WindowFlags_Visible | WindowFlags_Minimizable,
};

// @todo: convert to C ABI
enum WindowResetScenario_ __enum_type(uint32_t)
{
    WindowResetScenario_None        = 0x0,
    WindowResetScenario_Reload      = 0x1,
    WindowResetScenario_DeviceLost  = 0x2,
};

// @todo: convert to C ABI
typedef struct WindowDesc
{
    void*                   handle;
    void*                   graphicsContext;

    WindowFlags             flags;
    const char*             title;

    int32_t                 x;
    int32_t                 y;
    int32_t                 width;
    int32_t                 height;

    WindowResetScenario     resetScenario;
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

void                Window_RequestFocus(void);
void                Window_MakeGLCurrent(void);

bool                Window_HasInputFocus(void);
bool                Window_HasMouseFocus(void);

const WindowDesc*   Window_GetMainWindow(void);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
