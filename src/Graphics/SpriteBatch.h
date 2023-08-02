#pragma once

#include "Misc/Compiler.h"
#include <vectormath/vectormath_types.h>

struct Sprite;
struct SpriteSheet;

/// SpriteBatchState
enum SpriteBatchState __enum_type(uint32_t)
{
    SpriteBatchState_Idle,
    SpriteBatchState_Batching,
};

/// SpriteBatch
typedef struct SpriteBatch
{
    SpriteBatchState    state               __default_init(SpriteBatchState_Idle);
    
    uint32_t            textureId           __default_init(0);
    uint32_t            vertexArrayId       __default_init(0);
    uint32_t            verticesBufferId    __default_init(0);
    uint32_t            uvsBufferId         __default_init(0);
    uint32_t            colorsBufferId      __default_init(0);
    
    int32_t             count               __default_init(0);
    int32_t             capacity            __default_init(0);

    vec2*               vertices            __default_init(nullptr);
    vec2*               uvs                 __default_init(nullptr);
    vec3*               colors              __default_init(nullptr);
} SpriteBatch;

#ifdef __cplusplus
extern "C" {
#endif

void            SpriteBatch_Create(SpriteBatch* spriteBatch, const SpriteSheet* sheet, int32_t vertexCapacity);
void            SpriteBatch_Destroy(SpriteBatch* spriteBatch);

void            SpriteBatch_Begin(SpriteBatch* spriteBatch);
void            SpriteBatch_End(SpriteBatch* spriteBatch);

void            SpriteBatch_DrawSprite(SpriteBatch* spriteBatch, const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
