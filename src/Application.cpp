#include <stdio.h>
#include <float.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <vectormath.h>
#include <imgui/imgui.h>

#include "Application.h"

#include "Native/Input.h"
#include "Native/Window.h"
#include "Native/Thread.h"
#include "Native/Memory.h"

#include "Graphics/Graphics.h"

#include "Framework/Timer.h"
#include "Framework/JobSystem.h"

#include "Game/Game.h"

#include "ThirdPartyImpl/imgui_impl_sdl.h"
#include "ThirdPartyImpl/imgui_impl_opengl3.h"

static void Application_RenderProfiler(float deltaTime)
{
    char fpsText[1024];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.3f", deltaTime > FLOAT_EPSILON ? 1.0f / deltaTime : 0.0f);

    ImGui::Text(fpsText);

    ImGui::DumpMemoryAllocs();

    //vec2 fpsTextSize = vec2_mul1(Graphics::TextSize(fpsText), 2.0f);
    //
    //Graphics::DrawQuad(
    //    vec2_new(0.0f, (float)Window::GetHeight()),
    //    vec2_new(fpsTextSize.x + 10.0f, (float)Window::GetHeight() - 1.5f * fpsTextSize.y - 10.0f),
    //    vec3_new1(0.0f));
    //Graphics::DrawText(fpsText, vec2_new(5.0f, (float)Window::GetHeight() - 5.0f), vec3_new(0.25f, 0.5f, 1.0f));
}

static void Application_HandleWindowError()
{

}

static void Application_HandleRendererError(GraphicsError error)
{

}

int ApplicationMain(int argc, char* argv[])
{
    MEMORY_TRACKING();

    WindowDesc window;
    window.title    = "Pixel Adventure";
    window.x        = -1;
    window.y        = -1;
    window.width    = 448;
    window.height   = 256;
    window.flags    = WindowFlags::Default;

    Setup:
    {
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

        // Setup subsystems
        JobSystem::Setup();
        Input::Setup();

        Game::Setup();

        // Debugging and profiling
        IMGUI_CHECKVERSION();

        ImGui::SetAllocatorFunctions([](size_t size, void* _) {
            return MemoryAllocTag("ImGui", size, alignof(void*));
        }, [](void* ptr, void* _) {
            MemoryFreeTag("ImGui", ptr);
        });

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window.handle, (SDL_GLContext)window.graphicsContext);
        ImGui_ImplOpenGL3_Init();
    }

    ImGuiIO& io = ImGui::GetIO();
    MainLoop: while ((window.flags & WindowFlags::Quiting) == 0)
    {
        // Poll events
        Window::PollEvents();

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

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

#ifdef BUILD_PROFILING
        Application_RenderProfiler(deltaTime);
#endif

        // Rendering Imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        Graphics::Present();

        // Frame end
        Input::EndFrame();
        Timer::EndFrame();
    }

    Shutdown:
    {
        Game::Shutdown();

        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        // Shutdown subsystems
        Input::Shutdown();
        JobSystem::Shutdown();

        Graphics::Shutdown(&window);

        Window::Close(&window);
    }
    
    return 0;
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
