#pragma once

#include <stdint.h>
#include "Math/VectorTypes.h"

struct Sprite;
struct SpriteSheet;
struct SpriteBatch;

struct Sprite
{
    float           width       = 0.0f;
    float           height      = 0.0f;
    
    vec2            uv0         = vec2{0, 0};
    vec2            uv1         = vec2{0, 0};

    uint32_t        textureId   = 0;
};

struct SpriteSheet
{
    uint32_t        textureId   = 0;

    int32_t         cols        = 0;
    int32_t         rows        = 0;
    int32_t         spriteCount = 0;

    Sprite*         sprites     = nullptr;
};

enum struct GraphicsError : uint32_t
{
    None,
    InternalFatal,
    LoadDriverFailed,
    CreateContextFailed,
};

namespace Graphics
{
    GraphicsError   Setup(struct WindowDesc* window);
    void            Shutdown(struct WindowDesc* window);

    void            Clear(void);
    void            Present(void);

    bool            LoadSpriteSheet(SpriteSheet* spriteSheet, const char* file, int32_t cols, int32_t rows);
    void            UnloadSpriteSheet(SpriteSheet* spriteSheet);

    void            DrawSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color);
    void            DrawSpriteBatch(const SpriteBatch* spriteBatch);

    vec2            TextSize(const char* text);
    void            DrawText(const char* text, vec2 position, vec3 color);

    void            DrawQuad(vec2 start, vec2 end, vec3 color);
    void            DrawQuadLine(vec2 start, vec2 end, vec3 color);
}
