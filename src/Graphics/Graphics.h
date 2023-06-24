#pragma once

#include <stdint.h>
#include <vectormath/vectormath_types.h>

struct Sprite;
struct SpriteSheet;
struct SpriteBatch;
struct WindowDesc;

/// Sprite
/// Can be known as sub Texture
// @note: about default values, is all zero, so { 0 } fit the purpose
struct Sprite
{
    float           width       = 0.0f;
    float           height      = 0.0f;
    
    vec2            uv0         = vec2{0, 0};
    vec2            uv1         = vec2{0, 0};

    uint32_t        textureId   = 0;
};

/// SpriteSheet
/// Simple sprite list store in columns and rows
// @note: about default values, is all zero, so { 0 } fit the purpose
struct SpriteSheet
{
    uint32_t        textureId   = 0;

    int32_t         cols        = 0;
    int32_t         rows        = 0;
    
    int32_t         spriteCount = 0;
    Sprite*         sprites     = nullptr;
};

/// SrpiteAtlas
/// Batch multiple textures into one big texture
/// Can batch up to 8 textures
//struct SpriteAtlas
//{
// uint32_t             textureId   = 0;
// uint32_t             textures[8] = 0; // limits to 8 textures
//};

// @todo: convert to C ABI
// @note: sizeof enum is commonly int32_t/uint32_t (maybe at macros __enum_type(uint32_t))
enum struct GraphicsError : uint32_t
{
    None,
    InternalFatal,
    LoadDriverFailed,
    CreateContextFailed,
};

// @todo: convert to C ABI
namespace Graphics
{
    GraphicsError   Setup(WindowDesc* window);
    void            Shutdown(WindowDesc* window);

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
