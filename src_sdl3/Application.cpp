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
#include "Misc/Logging.h"

#include "ThirdPartyImpl/imgui_impl_sdl.h"
#include "ThirdPartyImpl/imgui_impl_opengl3.h"

LogStorage*     logStorage;
Logger          logStorageLogger;

static void Application_RenderDevTools(float deltaTime)
{
#ifdef BUILD_PROFILING
    ImGui::Begin("Frame", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    char fpsText[1024];
    snprintf(fpsText, sizeof(fpsText), "FPS: %.3f", deltaTime > FLOAT_EPSILON ? 1.0f / deltaTime : 0.0f);

    ImGui::Text(fpsText);

    //vec2 fpsTextSize = vec2_mul1(Graphics::TextSize(fpsText), 2.0f);
    //
    //Graphics::DrawQuad(
    //    vec2_new(0.0f, (float)Window::GetHeight()),
    //    vec2_new(fpsTextSize.x + 10.0f, (float)Window::GetHeight() - 1.5f * fpsTextSize.y - 10.0f),
    //    vec3_new1(0.0f));
    //Graphics::DrawText(fpsText, vec2_new(5.0f, (float)Window::GetHeight() - 5.0f), vec3_new(0.25f, 0.5f, 1.0f));

    ImGui::End();

    ImGui::Begin("Logs", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

    ImGui::Columns(3);
    ImGui::SetColumnWidth(0, 80);
    ImGui::SetColumnWidth(1, 120);

    ImGui::Text("Level");
    ImGui::NextColumn();
    ImGui::Text("Tag");
    ImGui::NextColumn();
    ImGui::Text("Text");
    ImGui::NextColumn();

    ImGui::Columns(1);
    if (ImGui::BeginChild("Records List"))
    {
        ImGui::Columns(3);
        ImGui::SetColumnWidth(0, 72);
        ImGui::SetColumnWidth(1, 120);
        for (LogRecord* record = logStorage->head; record != NULL; record = record->next)
        {
            ImGui::Text("%d", record->level);
            ImGui::NextColumn();
            ImGui::Text("%s", record->tag);
            ImGui::NextColumn();
            ImGui::Text("%s", record->text);
            ImGui::NextColumn();
        }
    }
    ImGui::EndChild();

    ImGui::End();
#endif

    ImGui::DumpMemoryAllocs();
}

static void Application_HandleWindowError()
{

}

static void Application_HandleRendererError(GraphicsError error)
{

}

int ApplicationMain(int argc, char* argv[])
{
    // Simple memory tracking
    MEMORY_TRACKING();

    WindowDesc window;
    window.title    = "Pixel Adventure";
    window.x        = -1;
    window.y        = -1;
    window.width    = 448;//1024;
    window.height   = 256;
    window.flags    = WindowFlags_Default;

    //Setup:
    {
        if (!Window_Open(&window))
        {
            Application_HandleWindowError();
            return -1;
        }

        const GraphicsError graphicsError = Graphics_Setup(&window);
        if (graphicsError != GraphicsError_None)
        {
            Application_HandleRendererError(graphicsError);
            Window_Close(&window);
            return -(int)graphicsError;
        }

        // Setup subsystems
        JobSystem::Setup();
        Input_Setup();

        if (!Game_Setup())
        {
            Window_Close(&window);
            return -10;
        }
    }

    // @todo: make DevTools module
    //DevTools:
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        //ImGuiStyle& style = ImGui::GetStyle();
        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //{
        //    style.WindowRounding = 0.0f;
        //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        //}

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window.handle, (SDL_GLContext)window.graphicsContext);
        ImGui_ImplOpenGL3_Init();
    }

    Window_RequestFocus();

    logStorage = LogStorage_Create(1024);
    logStorageLogger = LogStorage_GetLogger(logStorage);
    Log_AddLogger(&logStorageLogger);

    //MainLoop: 
    while ((window.flags & WindowFlags_Quiting) == 0)
    {
        // Poll events
        Window_PollEvents();

        // Handle window state
        if (window.resetScenario != WindowResetScenario_None)
        {
            // OpenGL handle device internal, but what about OpenGL ES?
            if (window.resetScenario & WindowResetScenario_DeviceLost)
            {
                window.resetScenario &= ~WindowResetScenario_DeviceLost;
                continue;
            }
        
            // When application awake or after device lost
            if (window.resetScenario & WindowResetScenario_Reload)
            {
                // Reload actually is unload then load again
                Game_Unload();
                Game_Load();
        
                window.resetScenario &= ~WindowResetScenario_Reload;
                continue;
            }
        
            // Application running smoothly
            window.resetScenario = WindowResetScenario_None;
            continue;
        }

        // Start new frame
        Timer_NewFrame();
        Input_NewFrame();
        
        const float totalTime = Timer_GetTotalTime();
        const float deltaTime = Timer_GetDeltaTime();
        Game_Update(totalTime, deltaTime);

        Graphics_Clear();
        
        Game_Render();

        //RenderDevTools:
        {
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            Application_RenderDevTools(deltaTime);
            Game_RenderDevTools();

            // Rendering Imgui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform 
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        Graphics_Present();

        // Frame end
        Input_EndFrame();
        Timer_EndFrame();
    }

    LogStorage_Destroy(logStorage);
    Log_SetLogger(Log_GetTTYLogger());

    //ShutdownDevTools:
    {
        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    //Shutdown:
    {
        Game_Shutdown();

        // Shutdown subsystems
        Input_Shutdown();
        JobSystem::Shutdown();

        Graphics_Shutdown(&window);

        Window_Close(&window);
    }
    
    return 0;
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
