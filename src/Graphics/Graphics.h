#pragma once

#include <stdint.h>
#include "Math/VectorTypes.h"

struct Sprite;
struct SpriteSheet;
struct SpriteBatch;

struct Sprite
{
    float           width;
    float           height;
    
    vec2            uv0, uv1;

    uint32_t        textureId;
};

struct SpriteSheet
{
    uint32_t        textureId;

    int32_t         cols;
    int32_t         rows;
    int32_t         spriteCount;

    Sprite*         sprites;
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
