#include <assert.h>
#include <stdlib.h>
#include <vectormath.h>

#include <stb/stb_image.h>
#include <stb/stb_easy_font.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "Graphics.h"
#include "SpriteBatch.h"

#include "Misc/Logging.h"
#include "Native/Window.h"
#include "Native/FileSystem.h"

static WindowDesc*      gMainWindow;
static SDL_GLContext    gGLContext;

static uint32_t         gProgramDrawText;
static uint32_t         gProgramDrawSprite;
static uint32_t         gProgramSpriteBatch;

static uint32_t         gVao;
static uint32_t         gVbo;
static uint32_t         gIbo;

static mat4             gProjection;

static const char* vshader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;"
    "out vec2 UV;"
    "uniform mat4 Projection;"
    "uniform mat4 Model;"
    "void main() {"
        "UV = vertex.zw;"
        "gl_Position = Projection * Model * vec4(vertex.xy, 0, 1.0);"
    "}";

static const char* fshader_src =
    "#version 330 core\n"
    "in vec2 UV;"
    "out vec4 FragColor;"
    "uniform vec3 Color;"
    "uniform sampler2D Image;"
    "void main() {"
        //"FragColor = vec4(Color, 1.0);"
        "FragColor = texture(Image, UV) * vec4(Color, 1.0);"
    "}";

static const char* vshader_draw_text_src =
    "#version 330 core\n"
    "layout (location = 0) in vec2 vertex;"
    "uniform mat4 Projection;"
    "uniform mat4 Model;"
    "void main() {"
        "gl_Position = Projection * Model * vec4(vertex.xy, 0, 1.0);"
    "}";

static const char* fshader_draw_text_src =
    "#version 330 core\n"
    "in vec2 UV;"
    "out vec4 FragColor;"
    "uniform vec3 Color;"
    "uniform sampler2D Image;"
    "void main() {"
        "FragColor = vec4(Color, 1.0);"
    "}";

static const char* vshader_sprite_batch_src =
    "#version 330 core\n"
    "layout (location = 0) in vec2 vertex;"
    "layout (location = 1) in vec2 uv;"
    "layout (location = 2) in vec3 color;"
    "out vec2 UV;"
    "out vec3 Color;"
    "uniform mat4 Projection;"
    "void main() {"
        "UV = uv;"
        "Color = color;"
        "gl_Position = Projection * vec4(vertex, 0, 1.0);"
    "}";

static const char* fshader_sprite_batch_src =
    "#version 330 core\n"
    "in vec2 UV;"
    "in vec3 Color;"
    "out vec4 FragColor;"
    "uniform sampler2D Image;"
    "void main() {"
        //"FragColor = vec4(Color, 1.0);"
        "FragColor = texture(Image, UV) * vec4(Color, 1.0);"
    "}";

static uint32_t CreateShader(uint32_t type, const char* src)
{
    uint32_t shader = glCreateShader(type);
    if (!shader)
    {
        //const char* error = (const char*)glewGetErrorString(glGetError());
        return 0;
    }

    glShaderSource(shader, 1, &src, 0);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shader, sizeof(errorLog), 0, errorLog);

        glDeleteShader(shader);
        return 0;
    }
    else
    {
        return shader;
    }
}

static uint32_t CreateProgram(const char* vshaderSource, const char* fshaderSource)
{
    uint32_t vshader = CreateShader(GL_VERTEX_SHADER, vshaderSource);
    uint32_t fshader = CreateShader(GL_FRAGMENT_SHADER, fshaderSource);

    uint32_t program = glCreateProgram();
    if (!program)
    {
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        return 0;
    }

    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char errorLog[1024];
        glGetProgramInfoLog(program, sizeof(errorLog), 0, errorLog);
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return program;
}

static void APIENTRY DebugOutput(
    GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam
) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    Log_Info("Graphics", "---------------");
    Log_Info("Graphics", "Debug message (%d): %s", id, message);

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             Log_Error("Graphics", "Source: API");              break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Log_Error("Graphics", "Source: Window System");    break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: Log_Error("Graphics", "Source: Shader Compiler");  break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     Log_Error("Graphics", "Source: Third Party");      break;
    case GL_DEBUG_SOURCE_APPLICATION:     Log_Error("Graphics", "Source: Application");      break;
    case GL_DEBUG_SOURCE_OTHER:           Log_Error("Graphics", "Source: Other");            break;
    };

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               Log_Error("Graphics", "Type: Error\n");                break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Log_Error("Graphics", "Type: Deprecated Behaviour\n"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Log_Error("Graphics", "Type: Undefined Behaviour\n");  break;
    case GL_DEBUG_TYPE_PORTABILITY:         Log_Error("Graphics", "Type: Portability\n");          break;
    case GL_DEBUG_TYPE_PERFORMANCE:         Log_Error("Graphics", "Type: Performance\n");          break;
    case GL_DEBUG_TYPE_MARKER:              Log_Error("Graphics", "Type: Marker\n");               break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          Log_Error("Graphics", "Type: Push Group\n");           break;
    case GL_DEBUG_TYPE_POP_GROUP:           Log_Error("Graphics", "Type: Pop Group\n");            break;
    case GL_DEBUG_TYPE_OTHER:               Log_Error("Graphics", "Type: Other\n");                break;
    };

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         Log_Error("Graphics", "Severity: high\n");            break;
    case GL_DEBUG_SEVERITY_MEDIUM:       Log_Error("Graphics", "Severity: medium\n");          break;
    case GL_DEBUG_SEVERITY_LOW:          Log_Error("Graphics", "Severity: low\n");             break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: Log_Error("Graphics", "Severity: notification\n");    break;
    };

    Log_Info("Graphics", "---------------\n");
}

GraphicsError Graphics::Setup(struct WindowDesc* window)
{
    assert(gMainWindow == NULL && gGLContext == NULL);

    // Create OpenGL Context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    gGLContext = SDL_GL_CreateContext((SDL_Window*)window->handle);
    if (!gGLContext)
    {
        return GraphicsError::CreateContextFailed;
    }

    // Load OpenGL Driver
    // @note: we can support Zink (GL on Vulkan), MGL (GL on metal) later
    if (!gladLoadGL())
    {
        return GraphicsError::LoadDriverFailed;
    }

    // Store main window for use later
    gMainWindow = window;
    gMainWindow->graphicsContext = gGLContext;

    // Bind debugger
#ifndef NDEBUG
    glDebugMessageCallback(DebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
    
    // Apply default settings
    glEnable(GL_BLEND);
    glBlendEquation(GL_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create default objects
    gProgramDrawText = CreateProgram(vshader_draw_text_src, fshader_draw_text_src);
    gProgramDrawSprite = CreateProgram(vshader_src, fshader_src);
    gProgramSpriteBatch = CreateProgram(vshader_sprite_batch_src, fshader_sprite_batch_src);

    glGenVertexArrays(1, &gVao);
    glGenBuffers(1, &gVbo);
    glGenBuffers(1, &gIbo);

    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0);

    glBindVertexArray(gIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo);

    return GraphicsError::None;
}

void Graphics::Shutdown(struct WindowDesc* window)
{
    if (gGLContext)
    {
        assert(window == gMainWindow);
        
        glDeleteProgram(gProgramDrawSprite);
        glDeleteProgram(gProgramDrawText);
        gProgramDrawSprite = 0;
        gProgramDrawText = 0;

        SDL_GL_DeleteContext(gGLContext);
        gGLContext  = nullptr;
        gMainWindow = nullptr;
    }
}

void Graphics::Clear()
{
    assert(gMainWindow != NULL && gGLContext != NULL);
    
    gProjection = mat4_ortho(0, (float)gMainWindow->width, 0, (float)gMainWindow->height, -10.0f, 10.0f);

    SDL_GL_MakeCurrent((SDL_Window*)gMainWindow->handle, gGLContext);
    glViewport(0, 0, gMainWindow->width, gMainWindow->height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::Present()
{
    assert(gMainWindow != NULL && gGLContext != NULL);

    SDL_GL_MakeCurrent((SDL_Window*)gMainWindow->handle, gGLContext);
    SDL_GL_SwapWindow((SDL_Window*)gMainWindow->handle);
}

bool Graphics::LoadSpriteSheet(SpriteSheet* spriteSheet, const char* file, int32_t cols, int32_t rows)
{
    glGenTextures(1, &spriteSheet->textureId);
    glBindTexture(GL_TEXTURE_2D, spriteSheet->textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Nearest are more better for pixel game
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Nearest are more better for pixel game

    char existsPath[1024];
    if (!FileSystem::GetExistsPath(existsPath, sizeof(existsPath), file))
    {
        return false;
    }

    int width, height, channels;
    void* pixels = stbi_load(existsPath, &width, &height, &channels, 0);
    if (!pixels)
    {
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
    
    int32_t spriteCount = cols * rows;
    spriteSheet->sprites = (Sprite*)malloc(spriteCount * sizeof(Sprite));
    spriteSheet->spriteCount = spriteCount;
    spriteSheet->cols = cols;
    spriteSheet->rows = rows;

    const float spriteWidth  = (float)width  / (float)cols;
    const float spriteHeight = (float)height / (float)rows;
    
    const float invCols = 1.0f / (float)cols;
    const float invRows = 1.0f / (float)rows;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Sprite* sprite = &spriteSheet->sprites[i * cols + j];
            sprite->textureId = spriteSheet->textureId;
            sprite->width = spriteWidth;
            sprite->height = spriteHeight;

            const vec2 uv0 = vec2_new((float) j      * invCols, (float)(i + 1) * invRows);
            const vec2 uv1 = vec2_new((float)(j + 1) * invCols, (float) i      * invRows);

            sprite->uv0 = uv0;
            sprite->uv1 = uv1;
        }
    }

    return true;
}

void Graphics::UnloadSpriteSheet(SpriteSheet* spriteSheet)
{
    glDeleteTextures(1, &spriteSheet->textureId);

    free(spriteSheet->sprites);

    spriteSheet->textureId = 0;

    spriteSheet->sprites = NULL;

    spriteSheet->cols = 0;
    spriteSheet->rows = 0;
    spriteSheet->spriteCount = 0;
}

void Graphics::DrawSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color)
{
    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), 0);

    const vec2 pos0 = vec2_new(-0.5f, -0.5f);
    const vec2 pos1 = vec2_new( 0.5f,  0.5f);
    const vec2 uv0  = sprite->uv0;
    const vec2 uv1  = sprite->uv1;

    const float vertices[] = {
        // pos              // tex
        pos0.x, pos0.y,     uv0.x, uv0.y,
        pos1.x, pos1.y,     uv1.x, uv1.y,
        pos0.x, pos1.y,     uv0.x, uv1.y,

        pos0.x, pos0.y,     uv0.x, uv0.y,
        pos1.x, pos0.y,     uv1.x, uv0.y,
        pos1.x, pos1.y,     uv1.x, uv1.y,
    };
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glUseProgram(gProgramDrawSprite);

    const mat4 model = mat4_transform2(position, rotation, vec2_mul(scale, vec2_new(sprite->width, sprite->height)));
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawSprite, "Model"), 1, false, (const float*)&model);
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawSprite, "Projection"), 1, false, (const float*)&gProjection);
    glUniform3f(glGetUniformLocation(gProgramDrawSprite, "Color"), color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite->textureId);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

vec2 Graphics::TextSize(const char* text)
{
    return vec2_mul1(vec2_new((float)stb_easy_font_width((char*)text), (float)stb_easy_font_height((char*)text)), 3.0f);
}

void Graphics::DrawText(const char* text, vec2 position, vec3 color)
{
    static float    vertices[4 * 10 * 1024]; // ~2000 chars
    static uint16_t indices[(sizeof(vertices) / sizeof(vertices[0])) / 4 * 6];
    
    const int quadsCount = stb_easy_font_print(0, 0, (char*)text, NULL, vertices, sizeof(vertices));
    const int vertexCount = quadsCount * 4;
    const int indexCount = quadsCount * 6;

    uint16_t* ptr = indices;
    for (int i = 0; i < quadsCount; i++)
    {
        uint16_t startIndex = i * 4;
        *ptr++ = startIndex + 0U;
        *ptr++ = startIndex + 1U;
        *ptr++ = startIndex + 2U;
        *ptr++ = startIndex + 0U;
        *ptr++ = startIndex + 2U;
        *ptr++ = startIndex + 3U;
    }
    
    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 4 * sizeof(float), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STREAM_DRAW);

    glUseProgram(gProgramDrawText);

    const vec2 drawPosition = vec2_sub(position, vec2_new(0.0f, 3.0f * stb_easy_font_height((char*)text)));
    const mat4 model = mat4_transform2(drawPosition, 0.0f, vec2_new(3.0f, -3.0f));
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawText, "Model"), 1, false, (const float*)&model);
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawText, "Projection"), 1, false, (const float*)&gProjection);
    glUniform3f(glGetUniformLocation(gProgramDrawText, "Color"), color.x, color.y, color.z);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);
}

void Graphics::DrawQuad(vec2 start, vec2 end, vec3 color)
{
    const vec2 pos0 = start;
    const vec2 pos1 = end;
    
    const vec2 uv0 = vec2_new1(0.0f);
    const vec2 uv1 = vec2_new1(0.0f);

    const float vertices[] = {
        // pos              // tex
        pos0.x, pos0.y,     uv0.x, uv0.y,
        pos1.x, pos1.y,     uv1.x, uv1.y,
        pos0.x, pos1.y,     uv0.x, uv1.y,

        pos0.x, pos0.y,     uv0.x, uv0.y,
        pos1.x, pos0.y,     uv1.x, uv0.y,
        pos1.x, pos1.y,     uv1.x, uv1.y,
    };

    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glUseProgram(gProgramDrawText);

    const mat4 model = mat4_identity();
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawText, "Model"), 1, false, (const float*)&model);
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawText, "Projection"), 1, false, (const float*)&gProjection);
    glUniform3f(glGetUniformLocation(gProgramDrawText, "Color"), color.x, color.y, color.z);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Graphics::DrawQuadLine(vec2 start, vec2 end, vec3 color)
{
    const vec2 pos0 = start;
    const vec2 pos1 = end;

    const vec2 uv0 = vec2_new1(0.0f);
    const vec2 uv1 = vec2_new1(0.0f);

    const float vertices[] = {
        // pos              // tex
        pos0.x, pos0.y,     uv0.x, uv0.y,
        pos0.x, pos1.y,     uv0.x, uv1.y,
        pos1.x, pos1.y,     uv1.x, uv1.y,
        pos1.x, pos0.y,     uv1.x, uv0.y,
        pos0.x, pos0.y,     uv0.x, uv0.y,
    };
    const int32_t verticesCount = (int32_t)(sizeof(vertices) / (sizeof(float) * 4));

    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glUseProgram(gProgramDrawText);

    const mat4 model = mat4_identity();
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawText, "Model"), 1, false, (const float*)&model);
    glUniformMatrix4fv(glGetUniformLocation(gProgramDrawText, "Projection"), 1, false, (const float*)&gProjection);
    glUniform3f(glGetUniformLocation(gProgramDrawText, "Color"), color.x, color.y, color.z);

    glDrawArrays(GL_LINE_STRIP, 0, verticesCount);
}

void Graphics::DrawSpriteBatch(const SpriteBatch* spriteBatch)
{
    assert(spriteBatch->state == SpriteBatchState_Idle);

    glBindVertexArray(spriteBatch->vertexArrayId);

    int vertexCount = spriteBatch->count * 6;

    glBindBuffer(GL_ARRAY_BUFFER, spriteBatch->verticesBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertexCount, spriteBatch->vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, spriteBatch->uvsBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertexCount, spriteBatch->uvs, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, spriteBatch->colorsBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertexCount, spriteBatch->colors, GL_DYNAMIC_DRAW);

    glUseProgram(gProgramSpriteBatch);
    glUniformMatrix4fv(glGetUniformLocation(gProgramSpriteBatch, "Projection"), 1, false, (const float*)&gProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, spriteBatch->textureId);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
