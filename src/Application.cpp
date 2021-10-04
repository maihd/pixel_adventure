#include <stdio.h>
#include <float.h>

#include "Application.h"

#include "Native/Window.h"
#include "Native/Thread.h"

#include "Graphics/Graphics.h"

#include "Framework/Timer.h"
#include "Framework/JobSystem.h"
#include "Framework/InputSystem.h"

#include "Game/Game.h"
#include "Math/VectorMathSimd.h"

static void Application_RenderProfiler(float deltaTime)
{
    char fpsText[1024];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.3f", deltaTime > FLT_EPSILON ? 1.0f / deltaTime : 0.0f);

    vec2 fpsTextSize = vec2_mul1(Graphics::TextSize(fpsText), 2.0f);

    Graphics::DrawQuad(
        vec2_new(0.0f, (float)Window_GetHeight()),
        vec2_new(fpsTextSize.x + 10.0f, (float)Window_GetHeight() - 1.5f * fpsTextSize.y - 10.0f),
        vec3_new1(0.0f));
    Graphics::DrawText(fpsText, vec2_new(5.0f, (float)Window_GetHeight() - 5.0f), vec3_new(0.25f, 0.5f, 1.0f));
}

static void Application_HandleWindowError()
{

}

static void Application_HandleRendererError(GraphicsError error)
{

}

int ApplicationMain(int argc, char* argv[])
{
    WindowDesc window;
    window.title = "Pixel Adventure";
    window.x = -1;
    window.y = -1;
    window.width = 448;
    window.height = 256;
    window.flags = WindowFlags_Default;
    if (!Window_Open(&window))
    {
        Application_HandleWindowError();
        return -1;
    }
    
    const GraphicsError graphicsError = Graphics::Setup(&window);
    if (graphicsError != GraphicsError::None)
    {
        Application_HandleRendererError(graphicsError);
        Window_Close(&window);
        return -1;
    }

    // Shutdown subsystems
    JobSystem_Setup();
    InputSystem_Setup();

    Game_Setup();

    while (Window_PollEvents())
    {
        // Handle window state
        //if (window.resetScenario != WindowResetScenario_None)
        //{
        //    if (window.resetScenario & WindowResetScenario_DeviceLost)
        //    {
        //        window.resetScenario &= ~WindowResetScenario_DeviceLost;
        //        continue;
        //    }
        //
        //    if (window.resetScenario & WindowResetScenario_Reload)
        //    {
        //        Game_Unload();
        //
        //        Game_Load();
        //
        //        window.resetScenario &= ~WindowResetScenario_Reload;
        //        continue;
        //    }
        //
        //    window.resetScenario = WindowResetScenario_None;
        //    continue;
        //}

        // Start new frame
        Timer_NewFrame();
        InputSystem_NewFrame();
        
        float totalTime = Timer_GetTotalTime();
        float deltaTime = Timer_GetDeltaTime();

        Game_Update(totalTime, deltaTime);

        Graphics::Clear();
        
        Game_Render();

        //Application_RenderProfiler(deltaTime);

        Graphics::Present();

        // Frame end
        InputSystem_EndFrame();
        Timer_EndFrame();
    }

    Game_Shutdown();

    // Shutdown subsystems
    InputSystem_Shutdown();
    JobSystem_Shutdown();
    
    Graphics::Shutdown(&window);

    Window_Close(&window);
    return 0;
}
