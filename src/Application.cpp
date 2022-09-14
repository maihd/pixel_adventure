#include <stdio.h>
#include <float.h>

#include "Application.h"

#include "Native/Input.h"
#include "Native/Window.h"
#include "Native/Thread.h"

#include "Graphics/Graphics.h"

#include "Framework/Timer.h"
#include "Framework/JobSystem.h"

#include "Game/Game.h"
#include <vectormath.h>

static void Application_RenderProfiler(float deltaTime)
{
    char fpsText[1024];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.3f", deltaTime > FLT_EPSILON ? 1.0f / deltaTime : 0.0f);

    vec2 fpsTextSize = vec2_mul1(Graphics::TextSize(fpsText), 2.0f);

    Graphics::DrawQuad(
        vec2_new(0.0f, (float)Window::GetHeight()),
        vec2_new(fpsTextSize.x + 10.0f, (float)Window::GetHeight() - 1.5f * fpsTextSize.y - 10.0f),
        vec3_new1(0.0f));
    Graphics::DrawText(fpsText, vec2_new(5.0f, (float)Window::GetHeight() - 5.0f), vec3_new(0.25f, 0.5f, 1.0f));
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
    window.flags = WindowFlags::Default;
    if (!Window::Open(&window))
    {
        Application_HandleWindowError();
        return -1;
    }
    
    const GraphicsError graphicsError = Graphics::Setup(&window);
    if (graphicsError != GraphicsError::None)
    {
        Application_HandleRendererError(graphicsError);
        Window::Close(&window);
        return -1;
    }

    // Shutdown subsystems
    JobSystem::Setup();
    Input::Setup();

    Game::Setup();

    while (Window::PollEvents())
    {
        // Handle window state
        if (window.resetScenario != WindowResetScenario::None)
        {
            if (window.resetScenario & WindowResetScenario::DeviceLost)
            {
                window.resetScenario &= ~WindowResetScenario::DeviceLost;
                continue;
            }
        
            if (window.resetScenario & WindowResetScenario::Reload)
            {
                Game::Unload();
        
                Game::Load();
        
                window.resetScenario &= ~WindowResetScenario::Reload;
                continue;
            }
        
            window.resetScenario = WindowResetScenario::None;
            continue;
        }

        // Start new frame
        Timer::NewFrame();
        Input::NewFrame();
        
        float totalTime = Timer::GetTotalTime();
        float deltaTime = Timer::GetDeltaTime();

        Game::Update(totalTime, deltaTime);

        Graphics::Clear();
        
        Game::Render();

        //Application_RenderProfiler(deltaTime);

        Graphics::Present();

        // Frame end
        Input::EndFrame();
        Timer::EndFrame();
    }

    Game::Shutdown();

    // Shutdown subsystems
    Input::Shutdown();
    JobSystem::Shutdown();
    
    Graphics::Shutdown(&window);

    Window::Close(&window);
    return 0;
}
