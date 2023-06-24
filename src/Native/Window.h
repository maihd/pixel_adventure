#pragma once

#include <stdint.h>
#include <assert.h> // For auto unclosed window attempt to be destroyed

// @todo: convert to C ABI
namespace WindowFlags
{
    enum Type : uint32_t
    {
        None          = 0,
        Visible       = 1 << 0,
        Resizable     = 1 << 1,
        Fullscreen    = 1 << 2,
        Borderless    = 1 << 3,

        Minimize      = 1 << 5,
        Maximize      = 1 << 6,
        Minimizable   = 1 << 7,
        Maximizable   = 1 << 8,

        Quiting       = 1 << 9,

        Default       = Visible | Minimizable,
    };
}

// @todo: convert to C ABI
namespace WindowResetScenario
{
    enum Type : uint32_t
    {
        None        = 0x0,
        Reload      = 0x1,
        DeviceLost  = 0x2,
    };
}

// @todo: convert to C ABI
struct WindowDesc
{
    void*               handle;
    void*               graphicsContext;

    uint32_t            flags;
    const char*         title;

    int32_t             x;
    int32_t             y;
    int32_t             width;
    int32_t             height;

    uint32_t            resetScenario;

    // @todo: do we need this?
    #ifdef __cplusplus
    inline WindowDesc()
        : handle(nullptr)
        , graphicsContext(nullptr)

        , flags(WindowFlags::None)
        , title("")
        , width(0)
        , height(0)
        , x(-1)
        , y(-1)
        , resetScenario(WindowResetScenario::None)
    {
    }

    inline ~WindowDesc()
    {
        assert(handle == nullptr);
    }
    #endif
};

// -------------------------------------------------------------
// Main functions
// -------------------------------------------------------------

// @todo: convert to C ABI
namespace Window
{
    bool                Open(WindowDesc* window);
    void                Close(WindowDesc* window);

    /// Process all coming events
    /// Return true if the window is alive, false otherwise, and the loop should shutdown
    bool                PollEvents(void);

    /// Wait a events and then process all coming events
    /// Return true if the window is alive, false otherwise, and the loop should shutdown
    bool                WaitAndPollEvents(void);

    float               GetWidth(void);
    float               GetHeight(void);

    bool                IsBorderless(void);
    bool                IsFullscreen(void);

    void                SetBorderless(void);
    void                SetFullscreen(void);

    bool                IsWindowed(void);
    void                SetWindowed(void);

    bool                IsVisible(void);
    void                SetVisible(bool visible);

    void                SetCenter(void);

    void                EnableVSync(void);
    void                DisableVSync(void);
    bool                IsVSyncEnabled(void);
    void                SetVSyncEnabled(bool value);

    void                RequestFocus(void);
    void                MakeGLCurrent(void);

    bool                HasInputFocus(void);
    bool                HasMouseFocus(void);

    const WindowDesc*   GetMainWindow(void);
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
