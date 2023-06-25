#pragma once

#include <stdint.h>
#include <vectormath/vectormath_types.h>

#include "Misc/Compiler.h"

struct Sprite;
struct SpriteSheet;
struct SpriteBatch;
struct WindowDesc;

/// Sprite
/// Can be known as sub Texture
typedef struct Sprite
{
    float           width       __default_init(0.0f);
    float           height      __default_init(0.0f);
    
    vec2            uv0         __default_init(vec2{0, 0});
    vec2            uv1         __default_init(vec2{0, 0});

    uint32_t        textureId   __default_init(0);
} Sprite;

/// SpriteSheet
/// Simple sprite list store in columns and rows
typedef struct SpriteSheet
{
    uint32_t        textureId   __default_init(0u);

    int32_t         cols        __default_init(0);
    int32_t         rows        __default_init(0);
    
    int32_t         spriteCount __default_init(0);
    Sprite*         sprites     __default_init(nullptr);
} SpriteSheet;

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
typedef enum GraphicsError __enum_type(uint32_t)
{
    GraphicsError_None,
    GraphicsError_InternalFatal,
    GraphicsError_LoadDriverFailed,
    GraphicsError_CreateContextFailed,
} GraphicsError;

#ifdef __cplusplus
extern "C" {
#endif

GraphicsError   Graphics_Setup(WindowDesc* window);
void            Graphics_Shutdown(WindowDesc* window);

void            Graphics_Clear(void);
void            Graphics_Present(void);

bool            Graphics_LoadSpriteSheet(SpriteSheet* spriteSheet, const char* file, int32_t cols, int32_t rows);
void            Graphics_UnloadSpriteSheet(SpriteSheet* spriteSheet);

void            Graphics_DrawSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color);
void            Graphics_DrawSpriteBatch(const SpriteBatch* spriteBatch);

vec2            Graphics_TextSize(const char* text);
void            Graphics_DrawText(const char* text, vec2 position, vec3 color);

void            Graphics_DrawQuad(vec2 start, vec2 end, vec3 color);
void            Graphics_DrawQuadLine(vec2 start, vec2 end, vec3 color);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
